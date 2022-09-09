#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ViewDataAsset.generated.h"

UCLASS()
class BIRDBRAWLER_API UViewDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Id = "";

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UViewBase> ViewWidget;
};
