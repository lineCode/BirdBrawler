#include "StateMachine.h"

UStateMachine::UStateMachine()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachine::BeginPlay()
{
	Super::BeginPlay();
}


void UStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
