#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

UCLASS()
class BIRDBRAWLER_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnShow();
	virtual void OnHide();
};
