#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "BirdBrawler/UI/UIViewsHandler.h"
#include "GameFlowStateBase.generated.h"

UENUM(BlueprintType)
enum UIViewsHandlerStateOutcome
{
	Ready,
	NotReady,
};

UCLASS()
class BIRDBRAWLER_API UGameFlowStateBase : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void Init_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ShowView(FString InId);

	UFUNCTION(BlueprintNativeEvent)
	void OnViewsHandlerReady();

protected:
	UPROPERTY()
	AUIViewsHandler* ViewsHandler = nullptr;

	bool ViewsHandlerReadyEventSent = false;
};
