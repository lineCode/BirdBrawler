#include "MultipleTargetsCameraComponent.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "BirdBrawler/Debug/Debug.h"
#include "Kismet/GameplayStatics.h"


UMultipleTargetsCameraComponent::UMultipleTargetsCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMultipleTargetsCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AutoAddTargetsOnBeginPlay)
	{
		AddAvailableTargets();
	}

	CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	verify(CameraComponent);
}

void UMultipleTargetsCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCameraPosition(DeltaTime);
	//UpdateCameraZoom(DeltaTime);
}

bool UMultipleTargetsCameraComponent::AddTarget(AActor* Target)
{
	if (Targets.Contains(Target))
	{
		return false;
	}

	Targets.Add(Target);
	return true;
}

bool UMultipleTargetsCameraComponent::RemoveTarget(AActor* Target)
{
	if (!Targets.Contains(Target))
	{
		return false;
	}

	Targets.Remove(Target);
	return true;
}

void UMultipleTargetsCameraComponent::AddAvailableTargets()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABirdBrawlerCharacter::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		AddTarget(Actor);
	}
}

FVector UMultipleTargetsCameraComponent::GetCenterPosition() const
{
	if (Targets.Num() == 0)
	{
		return GetOwner()->GetActorLocation();
	}

	if (Targets.Num() == 1)
	{
		const FVector MyLocation = GetOwner()->GetActorLocation();
		const FVector TargetLocation = Targets[0]->GetActorLocation();

		return FVector(MyLocation.X, TargetLocation.Y, TargetLocation.Z);
	}

	TArray<FVector> AllLocations;
	for (const AActor* Target : Targets)
	{
		AllLocations.Add(Target->GetActorLocation());
	}

	FVector Sum = AllLocations[0];
	for (int i = 1; i < AllLocations.Num(); ++i)
	{
		Sum += AllLocations[i];
	}

	Sum /= AllLocations.Num();
	return FVector(GetOwner()->GetActorLocation().X, Sum.Y, Sum.Z);
}

void UMultipleTargetsCameraComponent::UpdateCameraPosition(float DeltaTime) const
{
	GetOwner()->SetActorLocation(FMath::Lerp(GetOwner()->GetActorLocation(), GetCenterPosition() + PositionOffset,
	                                         MovementDamping * DeltaTime));
}

void UMultipleTargetsCameraComponent::UpdateCameraZoom(float DeltaTime) const
{
	FVector OutCenter, OutExtents;
	UGameplayStatics::GetActorArrayBounds(Targets, false, OutCenter, OutExtents);

	// TODO: is 0 always the right case for player id?
	if (const auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		const FVector BoundsTop = FVector(OutCenter.X, OutCenter.Y + OutExtents.Y / 2.f, OutCenter.Z);
		const FVector BoundsBottom = FVector(OutCenter.X, OutCenter.Y - OutExtents.Y / 2.f, OutCenter.Z);

		FVector2D OutScreenTop;
		const bool TopProjectionOk = PlayerController->ProjectWorldLocationToScreen(BoundsTop, OutScreenTop);

		FVector2D OutScreenBottom;
		const bool BottomProjectionOk = PlayerController->ProjectWorldLocationToScreen(BoundsBottom, OutScreenBottom);

		if (TopProjectionOk && BottomProjectionOk)
		{
			const FVector2D VerticalScreenDistance = OutScreenTop - OutScreenBottom;
			CameraComponent->SetFieldOfView(VerticalScreenDistance.Size());
		}
	}
}
