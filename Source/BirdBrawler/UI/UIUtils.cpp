#include "UIUtils.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Views/UIViewsHandler.h"

AUIViewsHandler* UIUtils::GetViewsHandler(const UWorld* World)
{
	AActor* ViewsHandlerActor = UGameplayStatics::GetActorOfClass(World, AUIViewsHandler::StaticClass());
	verify(ViewsHandlerActor);

	AUIViewsHandler* ViewsHandler = Cast<AUIViewsHandler>(ViewsHandlerActor);
	return ViewsHandler;
}

template <typename WidgetT, typename OwnerT>
WidgetT* UIUtils::CreateWidget(OwnerT* OwningObject, TSubclassOf<UUserWidget> UserWidgetClass, FName WidgetName /*= NAME_None*/)
{
	return CreateWidget<WidgetT>(OwningObject, UserWidgetClass, WidgetName);
}

void UIUtils::ShowWidget(UUserWidget* Widget, EWidgetShowTarget WidgetShowTarget /*= Viewport*/)
{
	verify(Widget);
	if (WidgetShowTarget == Viewport)
	{
		Widget->AddToViewport();
	}
	else
	{
		Widget->AddToPlayerScreen();
	}
}

void UIUtils::HideWidget(UUserWidget* Widget)
{
	verify(Widget);
	Widget->RemoveFromParent();
}
