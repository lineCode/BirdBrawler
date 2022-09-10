#pragma once

#include "CoreMinimal.h"

class AUIViewsHandler;

UENUM()
enum EWidgetShowTarget
{
	Viewport,
	PlayerScreen
};

class BIRDBRAWLER_API UIUtils
{
public:
	static AUIViewsHandler* GetViewsHandler(const UWorld* World);

	// TODO: fix this (unresolved external symbol when used)
	template <typename WidgetT = UUserWidget, typename OwnerT = UObject>
	static WidgetT* CreateWidget(OwnerT* OwningObject, TSubclassOf<UUserWidget> UserWidgetClass = WidgetT::StaticClass(), FName WidgetName = NAME_None);

	static void ShowWidget(UUserWidget* Widget, EWidgetShowTarget WidgetShowTarget = Viewport);
	static void HideWidget(UUserWidget* Widget);
};
