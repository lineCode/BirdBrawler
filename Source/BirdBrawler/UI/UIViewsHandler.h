#pragma once

#include "CoreMinimal.h"
#include "ViewDataAsset.h"
#include "GameFramework/Actor.h"
#include "UIViewsHandler.generated.h"

UCLASS()
class BIRDBRAWLER_API AUIViewsHandler : public AActor
{
	GENERATED_BODY()

public:
	AUIViewsHandler();

protected:
	UPROPERTY(EditAnywhere)
	TArray<UViewDataAsset*> ViewsDataAssets;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void ShowView(const FString& Id);
	UViewDataAsset* GetViewClassById(const FString& Id) const;
};
