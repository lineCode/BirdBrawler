﻿#pragma once

#include "CoreMinimal.h"
#include "HitboxDataAsset.h"
#include "MoveDataAsset.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HitboxNotifyState.generated.h"

UCLASS()
class BIRDBRAWLER_API UHitboxNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	UHitboxDataAsset* HitboxDataAsset;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                        float FrameDeltaTime) override;
};