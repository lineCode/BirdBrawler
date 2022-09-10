#include "UIViewsHandler.h"
#include "ViewBase.h"
#include "BirdBrawler/UI/UIUtils.h"
#include "Kismet/GameplayStatics.h"

AUIViewsHandler::AUIViewsHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUIViewsHandler::BeginPlay()
{
	Super::BeginPlay();

	InitViews();
}

void AUIViewsHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentView)
	{
		CurrentView->OnTick(DeltaTime);
	}
}

void AUIViewsHandler::InitViews()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	verify(PlayerController);

	for (UViewDataAsset* ViewDataAsset : ViewsDataAssets)
	{
		UViewBase* View = CreateWidget<UViewBase>(PlayerController, ViewDataAsset->ViewWidget);
		View->Id = ViewDataAsset->Id;

		Views.Emplace(View);
	}

	Ready = true;
}

void AUIViewsHandler::InvokeUIEvent(const FName& EventId)
{
	UIEventDelegate.Broadcast(EventId);
}

void AUIViewsHandler::ShowView(const FString& Id)
{
	UViewBase* View = GetViewById(Id);
	verify(View);

	if (CurrentView)
	{
		CurrentView->OnHide();
		UIUtils::HideWidget(CurrentView);
	}

	CurrentView = View;

	UIUtils::ShowWidget(CurrentView);
	CurrentView->OnShow();

	// TODO: Set input mode
}

UViewBase* AUIViewsHandler::GetViewById(const FString& Id) const
{
	auto* Element = Views.FindByPredicate([&](UViewBase* View) { return View->Id == Id; });
	if (Element == nullptr)
	{
		return nullptr;
	}

	return *Element;
}
