﻿#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "GameFlowStateBase.generated.h"

class AUIViewsHandler;

UCLASS()
class BIRDBRAWLER_API UGameFlowStateBase : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void Init_Implementation() override;
	virtual void Enter_Implementation() override;
	virtual void Exit_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ShowView(FString InId);

	UFUNCTION(BlueprintCallable)
	void LoadLevel(FName InLevelName);

	UFUNCTION(BlueprintNativeEvent)
	void OnViewsHandlerReady();

	UFUNCTION(BlueprintNativeEvent)
	void OnUIEvent(FName EventId);

protected:
	UPROPERTY()
	AUIViewsHandler* ViewsHandler = nullptr;

	bool ViewsHandlerReadyEventSent = false;

	void OnUIEventDelegate(FName EventId);

private:
	FDelegateHandle UIEventDelegateHandle;
};
