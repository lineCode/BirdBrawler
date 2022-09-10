#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "HUDInitialCountdown.generated.h"

UCLASS()
class BIRDBRAWLER_API UHUDInitialCountdown : public UWidgetBase
{
	GENERATED_BODY()

public:
	void DisplayRemainingSeconds(int RemainingSeconds);

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CountdownText = nullptr;
};
