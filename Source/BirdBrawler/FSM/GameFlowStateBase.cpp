#include "GameFlowStateBase.h"

#include "BirdBrawler/UI/UIUtils.h"
#include "BirdBrawler/UI/UIViewsHandler.h"

void UGameFlowStateBase::Init_Implementation()
{
	Super::Init_Implementation();

	ViewsHandler = UIUtils::GetViewsHandler(GetWorld());
}

void UGameFlowStateBase::Update_Implementation(float DeltaTime)
{
	Super::Update_Implementation(DeltaTime);

	if (!ViewsHandlerReadyEventSent && ViewsHandler && ViewsHandler->Ready)
	{
		ViewsHandlerReadyEventSent = true;
		OnViewsHandlerReady();
	}
}

void UGameFlowStateBase::ShowView(FString InId)
{
	verify(ViewsHandler);

	ViewsHandler->ShowView(InId);
}

void UGameFlowStateBase::OnViewsHandlerReady_Implementation()
{
}
