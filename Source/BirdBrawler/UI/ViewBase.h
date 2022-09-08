#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ViewBase.generated.h"

UCLASS()
class BIRDBRAWLER_API UViewBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FString Id = "";

	UFUNCTION(BlueprintNativeEvent)
	void OnShow();

	UFUNCTION(BlueprintNativeEvent)
	void OnHide();

	UFUNCTION(BlueprintNativeEvent)
	void OnTick(float DeltaTime);
};
