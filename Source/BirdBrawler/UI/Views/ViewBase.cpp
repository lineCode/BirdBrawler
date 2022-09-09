#include "ViewBase.h"

#include "UIViewsHandler.h"
#include "BirdBrawler/UI/UIUtils.h"

void UViewBase::OnShow_Implementation()
{
	ViewsHandler = UIUtils::GetViewsHandler(GetWorld());
	verify(ViewsHandler);
}

void UViewBase::OnHide_Implementation()
{
}

void UViewBase::OnTick_Implementation(float DeltaTime)
{
}

void UViewBase::InvokeUIEvent(FName EventId)
{
	ViewsHandler->InvokeUIEvent(EventId);
}
