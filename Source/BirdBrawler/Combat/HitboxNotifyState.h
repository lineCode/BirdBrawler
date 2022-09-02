#pragma once

#include "CoreMinimal.h"
#include "HitboxDataAsset.h"
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

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY()
	TArray<uint32> HitPawnsIds;
};
