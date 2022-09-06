#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HitboxDataAsset.generated.h"

UCLASS()
class BIRDBRAWLER_API UHitboxDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius{30.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackOrientation{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackForce{100.f};
};
