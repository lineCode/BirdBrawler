#include "MovesEffectorComponent.h"

UMovesEffectorComponent::UMovesEffectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovesEffectorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMovesEffectorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMovesEffectorComponent::ExecuteMove(const FVector& Knockback, const ABirdBrawlerCharacter& Target)
{
}
