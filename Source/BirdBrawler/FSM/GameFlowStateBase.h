#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "UObject/Object.h"
#include "GameFlowStateBase.generated.h"

UCLASS()
class BIRDBRAWLER_API UGameFlowStateBase : public UStateBase
{
	GENERATED_BODY()

public:
	void ShowView(const FString& Id);
};
