#pragma once

#include "CoreMinimal.h"
#include "HitboxDataAsset.h"
#include "Components/ActorComponent.h"
#include "MovesEffectorComponent.generated.h"

struct FHitboxData
{
	const UHitboxDataAsset* HitboxDataAsset{nullptr};
	const UWorld* World{nullptr};
	AActor* Owner{nullptr};
	FVector Location;
	uint32 Id;
	TArray<uint32> HitPawnsIds;
};

class ABirdBrawlerCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BIRDBRAWLER_API UMovesEffectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovesEffectorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// TODO: this will not only hit ABirdBrawlerCharacter but also other entities (make hittable entity?)
	void EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, const UWorld* World, AActor* Owner, const FVector& Location, uint32 Id);
	void DisableHitbox(uint32 Id);

protected:
	TArray<FHitboxData> ActiveHitboxes;

	virtual void BeginPlay() override;

	void ApplyHitboxData(FHitboxData& HitboxData) const;

private:
	void RemoveHitboxDataById(uint32 Id);
};
