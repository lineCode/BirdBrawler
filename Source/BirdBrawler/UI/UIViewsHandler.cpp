#include "UIViewsHandler.h"
#include "ViewBase.h"
#include "Kismet/GameplayStatics.h"

AUIViewsHandler::AUIViewsHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUIViewsHandler::BeginPlay()
{
	Super::BeginPlay();
}

void AUIViewsHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUIViewsHandler::ShowView(const FString& Id)
{
	UViewDataAsset* ViewDataAsset = GetViewClassById(Id);
	verify(ViewDataAsset);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UViewBase* View = CreateWidget<UViewBase>(PlayerController, ViewDataAsset->ViewWidget);
	View->AddToViewport();

	// TODO: Set input mode
}

UViewDataAsset* AUIViewsHandler::GetViewClassById(const FString& Id) const
{
	return *ViewsDataAssets.FindByPredicate([&](UViewDataAsset* DataAsset) { return DataAsset->Id == Id; });
}
