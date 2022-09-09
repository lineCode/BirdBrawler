#include "GameFlowStateBase.h"
#include "BirdBrawler/UI/UIUtils.h"
#include "BirdBrawler/UI/UIViewsHandler.h"
#include "Kismet/GameplayStatics.h"

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

void UGameFlowStateBase::Enter_Implementation()
{
	Super::Enter_Implementation();

	UIEventDelegateHandle = ViewsHandler->UIEventDelegate.AddUObject(this, &UGameFlowStateBase::OnUIEventDelegate);
}

void UGameFlowStateBase::Exit_Implementation()
{
	Super::Exit_Implementation();

	ViewsHandler->UIEventDelegate.Remove(UIEventDelegateHandle);
}

void UGameFlowStateBase::ShowView(FString InId)
{
	verify(ViewsHandler);

	ViewsHandler->ShowView(InId);
}

void UGameFlowStateBase::LoadLevel(FName InLevelName)
{
	UGameplayStatics::OpenLevel(this, InLevelName, true);
}

void UGameFlowStateBase::OnUIEventDelegate(FName EventId)
{
	OnUIEvent(EventId);
}

void UGameFlowStateBase::OnUIEvent_Implementation(FName EventId)
{
}

void UGameFlowStateBase::OnViewsHandlerReady_Implementation()
{
}
