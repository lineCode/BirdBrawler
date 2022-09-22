// Copyright (c) Giammarco Agazzotti

#pragma once

#include "CoreMinimal.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "GameFramework/Actor.h"
#include "HitStunHandler.generated.h"

struct FHitStunData
{
	explicit FHitStunData(ABirdBrawlerCharacter* InTargetCharacter, float InRemainingTime, bool InShake) :
		TargetCharacter(InTargetCharacter),
		RemainingTime(InRemainingTime),
		Shake(InShake),
		OriginalMeshRelativeLocation(InTargetCharacter->GetSkeletalMeshComponent()->GetRelativeLocation()),
		Dirty(false)
	{
	}

	ABirdBrawlerCharacter* TargetCharacter;

	float RemainingTime;
	bool Shake;

	FVector OriginalMeshRelativeLocation;
	bool Dirty;
};

UCLASS()
class BIRDBRAWLER_API AHitStunHandler : public AActor
{
	GENERATED_BODY()

public:
	AHitStunHandler();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float MeshShakeFrequency = 2.5f;

	UPROPERTY(EditAnywhere)
	float MeshShakeAmplitude = 60.f;

public:
	virtual void Tick(float DeltaTime) override;

	void ApplyHitStun(ABirdBrawlerCharacter* Target, float Duration, bool Shake = false);

private:
	TArray<FHitStunData> TargetsTimeTracker;

	FHitStunData* GetHitStunData(uint32 TargetId);
	void UpdateTargetShake(ABirdBrawlerCharacter* Target);
	void StopTargetShake(const FHitStunData& HitStunData);
};
