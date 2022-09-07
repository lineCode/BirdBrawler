#pragma once

#include "CoreMinimal.h"
#include "HitboxDataAsset.h"
#include "Components/ActorComponent.h"
#include "HitboxData.h"
#include "MovesEffectorComponent.generated.h"

class ABirdBrawlerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BIRDBRAWLER_API UMovesEffectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovesEffectorComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// TODO: this will not only hit ABirdBrawlerCharacter but also other entities (make hittable entity?)
	void EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, bool ForceOpponentFacing, float DamagePercent, const UWorld* World, AActor* Owner, const FVector& Location,
	                  uint32 Id);

	void EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, bool ForceOpponentFacing, float DamagePercent, const UWorld* World, AActor* Owner,
	                  USkeletalMeshComponent* SkeletalMesh, const FName& SocketToFollow, uint32 Id);

	void DisableHitbox(uint32 Id);

	FHitboxData* GetHitboxData(uint32 Id);

	FVector CalculateKnockbackVector(const UHitboxDataAsset* HitboxDataAsset) const;

protected:
	TArray<FHitboxData> ActiveHitboxes;

	UPROPERTY()
	ABirdBrawlerCharacter* Character{nullptr};

	void ApplyHitboxData(FHitboxData& HitboxData) const;

private:
	void RemoveHitboxDataById(uint32 Id);
};
