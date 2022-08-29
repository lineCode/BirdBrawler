// Copyright Epic Games, Inc. All Rights Reserved.

#include "BirdBrawlerCharacter.h"

#include "BirdBrawler/Debug/Debug.h"
#include "BirdBrawler/Combat/MovesEffectorComponent.h"
#include "BirdBrawler/Animation/CharacterAnimInstance.h"
#include "BirdBrawler/Combat/IHittable.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	verify(Fsm);
	Fsm->Start();
	Fsm->PushState("Idle");
}

void ABirdBrawlerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABirdBrawlerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Airborne = GetCharacterMovement()->IsFalling();

	BB_SLOGK(10, FString::Printf(TEXT("Movement direction: %f"), MovementDirection));
}

void ABirdBrawlerCharacter::OnHit(const FVector& Knockback, const AActor& Hitter)
{
	IHittable::OnHit(Knockback, Hitter);

	BB_SLOG_ERR("REACTION");

	GoToFsmState("LightReaction");
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

void ABirdBrawlerCharacter::EvaluateHitResult(const FHitResult& HitResult)
{
	const AActor* HitActor = HitResult.GetActor();
	BB_SLOG(HitActor->GetName());
}
