#pragma once

#include "CoreMinimal.h"
#include "HitboxDataAsset.h"
#include "Components/ActorComponent.h"
#include "HitboxData.h"
#include "HitStunHandler.h"
#include "MovesEffectorComponent.generated.h"

class ABirdBrawlerCharacter;

using HitboxActorPair = TPair<uint32, TArray<uint32>>;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BIRDBRAWLER_API UMovesEffectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovesEffectorComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// TODO: this will not only hit ABirdBrawlerCharacter but also other entities (make hittable entity?)
	void EnableHitbox(FHitboxData&& HitboxData);
	void DisableHitbox(uint32 Id);

	FHitboxData* GetHitboxData(uint32 Id);

	FVector CalculateKnockbackVector(const UHitboxDataAsset* HitboxDataAsset) const;

protected:
	TArray<FHitboxData> ActiveHitboxes;

	UPROPERTY()
	ABirdBrawlerCharacter* Character{nullptr};

	void ActivateHitbox(FHitboxData& HitboxData);

	bool CharacterWasHitPreviously(uint32 CharacterUniqueId, uint32 HitBoxId);
	void RegisterHitCharacter(uint32 CharacterUniqueId, uint32 HitBoxId);

private:
	TArray<HitboxActorPair> HitboxHitActorPairs;

	UPROPERTY()
	AHitStunHandler* HitStunHandler = nullptr;

	void RemoveHitboxDataById(uint32 Id);
};
