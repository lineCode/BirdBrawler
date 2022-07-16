#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MoveDataAsset.generated.h"

UCLASS()
class BIRDBRAWLER_API UMoveDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimationAsset* AnimationAsset{nullptr};
};
