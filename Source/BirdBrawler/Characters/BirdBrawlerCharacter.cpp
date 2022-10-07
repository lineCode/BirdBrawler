#include "BirdBrawlerCharacter.h"

#include "BirdBrawler/Combat/MovesEffectorComponent.h"
#include "BirdBrawler/Animation/CharacterAnimInstance.h"
#include "BirdBrawler/Combat/CombatUtils.h"
#include "BirdBrawler/Combat/IHittable.h"
#include "BirdBrawler/Common/Tags.h"
#include "BirdBrawler/Debug/Debug.h"
#include "BirdBrawler/UI/Widgets/Character/CharacterHUDWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABirdBrawlerCharacter::ABirdBrawlerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	Fsm = CreateDefaultSubobject<UFSM>(TEXT("ActionsFSM"));

	MovesBufferComponent = CreateDefaultSubobject<UMovesBufferComponent>(TEXT("MovesBuffer"));
	MovesEffectorComponent = CreateDefaultSubobject<UMovesEffectorComponent>(TEXT("MovesEffector"));

	SkeletalMeshComponent = FindComponentByClass<USkeletalMeshComponent>();
}

void ABirdBrawlerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitFsm();
	InitMaterialInstances();
	InitHUD();
	InitTimeDilations();
	InitPushBox();
	InitWallBox();

	SetMaterials(InitialMaterialInstances);
	SetInvincibilityMaterialsParameters();

	CachedAirControl = GetCharacterMovement()->AirControl;
}

void ABirdBrawlerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABirdBrawlerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Airborne = GetCharacterMovement()->IsFalling();

	UpdatePushboxOverlap(DeltaSeconds);
}

void ABirdBrawlerCharacter::OnHit(const FVector& Knockback, float PitchDegreesAbs, float KnockbackForce, AActor* Hitter)
{
	IHittable::OnHit(Knockback, PitchDegreesAbs, KnockbackForce, Hitter);

	// TODO: remove magic numbers, this is just for testing purposes
	if (PitchDegreesAbs > 15.f && KnockbackForce > 500.f)
	{
		GoToFsmState("LaunchStart");
	}
	else
	{
		GoToFsmState("LightReaction");
	}
}

void ABirdBrawlerCharacter::MoveHorizontal(float Value)
{
	MovementDirection = Value;

	AddMovementInput(FVector(0.f, -1.f, 0.f), MovementDirection);
}

void ABirdBrawlerCharacter::GoToFsmState(const FName StateName)
{
	Fsm->PopActiveState();
	Fsm->PushState(StateName);
}

float ABirdBrawlerCharacter::GetMovementDirection() const
{
	return MovementDirection;
}

void ABirdBrawlerCharacter::SetMovementDirection(float Direction)
{
	MovementDirection = Direction;
}

float ABirdBrawlerCharacter::GetMovementRotationYaw() const
{
	return MovementDirection > 0.f ? -90.f : 90.f;
}

bool ABirdBrawlerCharacter::IsAirborne() const
{
	return Airborne;
}

FName ABirdBrawlerCharacter::GetCurrentMove() const
{
	return CurrentMove;
}

void ABirdBrawlerCharacter::SetCurrentMove(FName MoveName)
{
	CurrentMove = MoveName;

	InvokeCurrentMoveChangedDelegate(MoveName);

	UCharacterAnimInstance* CharacterAnimInstance = Cast<UCharacterAnimInstance>(
		GetSkeletalMeshComponent()->GetAnimInstance());

	verify(CharacterAnimInstance);

	CharacterAnimInstance->CurrentMoveChanged(MoveName);
}

float ABirdBrawlerCharacter::GetInputMovement() const
{
	return GetMovesBufferComponent()->InputMovement;
}

void ABirdBrawlerCharacter::InvokeMoveEndedDelegate(FName MoveName) const
{
	MoveEndedDelegate.Broadcast(MoveName);
}

void ABirdBrawlerCharacter::InvokeCurrentMoveChangedDelegate(FName MoveName) const
{
	CurrentMoveChangedDelegate.Broadcast(MoveName);
}

void ABirdBrawlerCharacter::PlayAnimation(UAnimationAsset* AnimationAsset, bool Loop /*= false*/) const
{
	GetSkeletalMeshComponent()->PlayAnimation(AnimationAsset, Loop);
}

void ABirdBrawlerCharacter::PlayAnimationSequence(UAnimSequenceBase* AnimationSequence, bool Loop /*= false*/) const
{
	UCharacterAnimInstance* CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetSkeletalMeshComponent()->GetAnimInstance());

	verify(CharacterAnimInstance);

	CharacterAnimInstance->AnimationRequested(AnimationSequence, Loop);
}

void ABirdBrawlerCharacter::Push(EPushDirection Direction, float Intensity)
{
	FVector FinalVector = Direction == EPushDirection::Front
		                      ? (IsFacingRight() ? FVector::LeftVector : FVector::RightVector)
		                      : (IsFacingRight() ? FVector::RightVector : FVector::LeftVector);

	LaunchCharacter(FinalVector * Intensity, true, true);
}

bool ABirdBrawlerCharacter::IsFacingRight() const
{
	return FMath::IsNearlyEqual(GetActorRotation().Yaw, -90.f, 2.f);
}

void ABirdBrawlerCharacter::SetInvincible(bool InInvincible, bool InAllowDamage)
{
	Invincible = InInvincible;
	InvincibleAllowDamage = InAllowDamage;

	SetMaterials(Invincible ? EditableMaterialInstances : InitialMaterialInstances);
}

void ABirdBrawlerCharacter::EnableHitStun(bool Shake)
{
	TimeDilations.Push(MIN_HITSTUN_TIME_DILATION);

	CustomTimeDilation = TimeDilations.Last();
	ShakeMesh = Shake;
}

void ABirdBrawlerCharacter::DisableHitStun()
{
	TimeDilations.Pop();
	verify(TimeDilations.Num() > 0);

	CustomTimeDilation = TimeDilations.Last();
}

void ABirdBrawlerCharacter::ApplyAirFrictionIncrease(float ElapsedTimeSinceAirborne)
{
	auto* MovementComponent = GetMovementComponent();
	if (MovementComponent && IsAirborne())
	{
		FVector TargetVelocity = MovementComponent->Velocity;
		float VelocityMultiplier = LaunchAirFrictionIncreaseCurve.GetRichCurve()->Eval(ElapsedTimeSinceAirborne);

		TargetVelocity.Y *= VelocityMultiplier;

		MovementComponent->Velocity = TargetVelocity;
	}
}

void ABirdBrawlerCharacter::ClearMovesBuffer()
{
	GetMovesBufferComponent()->ClearBuffer();
}

float ABirdBrawlerCharacter::GetKnockbackMultiplier() const
{
	return KnockbackMultiplierCurve.GetRichCurveConst()->Eval(DamagePercent);
}

bool ABirdBrawlerCharacter::IsAgainstWall(FWallCollisionInfo& OutWallCollisionInfo) const
{
	if (WallBox != nullptr)
	{
		TArray<UPrimitiveComponent*> OverlappingComponents;
		WallBox->GetOverlappingComponents(OverlappingComponents);

		if (OverlappingComponents.Num() > 0)
		{
			OutWallCollisionInfo.DidCollide = true;
			OutWallCollisionInfo.Wall = OverlappingComponents[0]->GetOwner();

			return true;
		}
	}

	return false;
}

void ABirdBrawlerCharacter::EnableAirMovement(bool Enable)
{
	if (Enable)
	{
		GetCharacterMovement()->AirControl = CachedAirControl;
	}
	else
	{
		CachedAirControl = GetCharacterMovement()->AirControl;
		GetCharacterMovement()->AirControl = 0.f;
	}
}

void ABirdBrawlerCharacter::OnWallCollision_Implementation(FVector CollisionNormal)
{
}

void ABirdBrawlerCharacter::InitFsm()
{
	verify(Fsm);

	Fsm->Start();
	Fsm->PushState("Idle");
}

void ABirdBrawlerCharacter::InitMaterialInstances()
{
	verify(SkeletalMeshComponent);

	TArray<UMaterialInterface*> Materials = SkeletalMeshComponent->GetMaterials();
	for (UMaterialInterface* Material : Materials)
	{
		InitialMaterialInstances.Emplace(UMaterialInstanceDynamic::Create(Material, this));
		EditableMaterialInstances.Emplace(UMaterialInstanceDynamic::Create(Material, this));
	}
}

void ABirdBrawlerCharacter::InitHUD()
{
	CharacterHUDWidget = FindComponentByClass<UWidgetComponent>();
	if (CharacterHUDWidget)
	{
		if (auto* HUDWidget = Cast<UCharacterHUDWidget>(CharacterHUDWidget->GetUserWidgetObject()))
		{
			HUDWidget->SetOwner(this);
		}
	}
}

void ABirdBrawlerCharacter::InitTimeDilations()
{
	TimeDilations.Empty();
	TimeDilations.Push(CustomTimeDilation);
}

void ABirdBrawlerCharacter::InitPushBox()
{
	TArray<UActorComponent*> PushBoxes = GetComponentsByTag(UBoxComponent::StaticClass(), TAG_PUSHBOX);
	ensure(PushBoxes.Num() <= 1);

	if (PushBoxes.Num() > 0)
	{
		PushBox = Cast<UBoxComponent>(PushBoxes[0]);
		if (PushBox == nullptr)
		{
			BB_SLOG_WARN(FString::Printf(TEXT("Character [%s] has no Pushbox available; will overlap with other characters"), *GetName()));
		}
	}
}

void ABirdBrawlerCharacter::InitWallBox()
{
	TArray<UActorComponent*> WallBoxes = GetComponentsByTag(UBoxComponent::StaticClass(), TAG_WALLBOX);
	ensure(WallBoxes.Num() <= 1);

	if (WallBoxes.Num() > 0)
	{
		WallBox = Cast<UBoxComponent>(WallBoxes[0]);
		if (WallBox == nullptr)
		{
			BB_SLOG_WARN(FString::Printf(TEXT("Character [%s] has no Wallbox available; will overlap with other characters"), *GetName()));
		}
	}
}

void ABirdBrawlerCharacter::SetMaterials(TArray<UMaterialInstanceDynamic*>& Materials)
{
	verify(SkeletalMeshComponent);

	for (int32 i = 0; i < Materials.Num(); ++i)
	{
		SkeletalMeshComponent->SetMaterial(i, Materials[i]);
	}
}

void ABirdBrawlerCharacter::SetInvincibilityMaterialsParameters()
{
	for (UMaterialInstanceDynamic* Material : EditableMaterialInstances)
	{
		Material->SetScalarParameterValue("Frequency", InvincibilityMaterialPulseFrequency);
		Material->SetScalarParameterValue("Intensity", InvincibilityMaterialPulseIntensity);
	}
}

void ABirdBrawlerCharacter::OnHitStunTimerEnded()
{
	ShakeMesh = false;
	CustomTimeDilation = 1.f;
}

void ABirdBrawlerCharacter::UpdatePushboxOverlap(float DeltaTime)
{
	if (PushBox != nullptr)
	{
		TArray<UPrimitiveComponent*> OverlappingComponents;
		PushBox->GetOverlappingComponents(OverlappingComponents);

		// TODO: temp, let's start by handling a single other enemy
		if (OverlappingComponents.Num() == 1)
		{
			FWallCollisionInfo WallCollisionInfo;
			IsAgainstWall(WallCollisionInfo);

			AActor* OtherActor = OverlappingComponents[0]->GetOwner();
			bool OtherOnTheRight = OtherActor->GetActorLocation().Y > GetActorLocation().Y;

			bool WallCollisionBlocksMovement = false;
			if (WallCollisionInfo.DidCollide)
			{
				bool WallOnTheRight = WallCollisionInfo.Wall->GetActorLocation().Y > GetActorLocation().Y;
				WallCollisionBlocksMovement = (OtherOnTheRight && !WallOnTheRight) || (!OtherOnTheRight && WallOnTheRight);
			}

			if (!WallCollisionBlocksMovement)
			{
				float MyShiftingMultiplier = OtherOnTheRight ? -1.f : 1.f;

				const FVector CurrentLocation = GetActorLocation();
				const float NextPosition = CurrentLocation.Y + (PushboxShiftRatePerFrame * MyShiftingMultiplier * DeltaTime);
				const FVector NextLocation = FVector(CurrentLocation.X, NextPosition, CurrentLocation.Z);

				SetActorLocation(NextLocation);
			}
		}
	}
}

void ABirdBrawlerCharacter::UpdateWallsOverlap()
{
	if (WallBox != nullptr)
	{
		TArray<UPrimitiveComponent*> OverlappingComponents;
		WallBox->GetOverlappingComponents(OverlappingComponents);

		// TODO: this may not take into account all possibilities
		// this whole thing could be solved by using collision events, figure out how to do it
		if (CollidingWalls != OverlappingComponents)
		{
			if (OverlappingComponents.Num() > CollidingWalls.Num())
			{
			}
		}
	}
}
