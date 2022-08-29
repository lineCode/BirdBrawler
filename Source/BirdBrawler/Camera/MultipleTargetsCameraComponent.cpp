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

	UpdateCameraPosition();
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABirdBrawlerCharacter::StaticClass(), Targets);
}

FVector UMultipleTargetsCameraComponent::GetCenterPosition() const
{
	FVector Center = GetOwner()->GetActorLocation();
	if (Targets.Num() > 0)
	{
		FVector BoxExtents;
		UGameplayStatics::GetActorArrayBounds(Targets, false, Center, BoxExtents);

		const FVector TargetLocation = FVector(GetOwner()->GetActorLocation().X, Center.Y, Center.Z);
		Center = TargetLocation;
	}

	return Center;
}

void UMultipleTargetsCameraComponent::UpdateCameraPosition() const
{
	GetOwner()->SetActorLocation(GetCenterPosition());
}
