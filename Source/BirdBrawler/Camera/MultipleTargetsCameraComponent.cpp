#include "MultipleTargetsCameraComponent.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
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
}

void UMultipleTargetsCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCameraPosition(DeltaTime);
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
		return Targets[0]->GetActorLocation();
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
