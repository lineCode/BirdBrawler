#pragma once

#include "CoreMinimal.h"
#include "MoveDataAsset.h"
#include "Components/ActorComponent.h"
#include "MovesEffectorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BIRDBRAWLER_API UMovesEffectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovesEffectorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ExecuteMove(UMoveDataAsset* MoveDataAsset);
};
