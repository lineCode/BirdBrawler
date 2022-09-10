#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "HUDCountdown.generated.h"

UCLASS()
class BIRDBRAWLER_API UHUDCountdown : public UWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TimerText = nullptr;
};
