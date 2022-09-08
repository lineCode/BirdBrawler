#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "GameFlowStateBase.generated.h"

UCLASS()
class BIRDBRAWLER_API UGameFlowStateBase : public UStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowView(FString InId);
};
