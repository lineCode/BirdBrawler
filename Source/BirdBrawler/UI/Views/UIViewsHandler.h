#pragma once

#include "CoreMinimal.h"
#include "ViewDataAsset.h"
#include "GameFramework/Actor.h"
#include "UIViewsHandler.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FUIEvent, FName);

UCLASS()
class BIRDBRAWLER_API AUIViewsHandler : public AActor
{
	GENERATED_BODY()

public:
	AUIViewsHandler();

	void InvokeUIEvent(const FName& EventId);

protected:
	UPROPERTY(EditAnywhere)
	TArray<UViewDataAsset*> ViewsDataAssets;

	UPROPERTY()
	UViewBase* CurrentView = nullptr;

	UPROPERTY()
	TArray<UViewBase*> Views;

	virtual void BeginPlay() override;

	void InitViews();

public:
	bool Ready = false;
	FUIEvent UIEventDelegate;

	virtual void Tick(float DeltaTime) override;

	void ShowView(const FString& Id);
	UViewBase* GetViewById(const FString& Id) const;
};
