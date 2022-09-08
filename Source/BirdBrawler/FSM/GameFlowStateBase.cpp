#include "GameFlowStateBase.h"

#include "BirdBrawler/UI/UIUtils.h"
#include "BirdBrawler/UI/UIViewsHandler.h"

void UGameFlowStateBase::ShowView(FString InId)
{
	AUIViewsHandler* ViewsHandler = UIUtils::GetViewsHandler(GetWorld());
	verify(ViewsHandler);

	ViewsHandler->ShowView(InId);
}
