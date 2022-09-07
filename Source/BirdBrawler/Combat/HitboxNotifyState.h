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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForceOpponentFacing = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePercent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IgnoreKnockbackMultiplier = false;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY()
	TArray<uint32> HitPawnsIds;
};
