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
	USkeletalMeshComponent* SkeletalMesh{nullptr};
	FName SocketToFollow{""};
	uint32 Id;
	TArray<uint32> HitPawnsIds;

	FHitboxData(const UHitboxDataAsset* InHitboxDataAsset, const UWorld* InWorld, AActor* InOwner, USkeletalMeshComponent* InSkeletalMesh, const FName& InSocketToFollow,
	            uint32 InId)
		: HitboxDataAsset(InHitboxDataAsset), World(InWorld), Owner(InOwner), SkeletalMesh(InSkeletalMesh), SocketToFollow(InSocketToFollow), Id(InId)
	{
	}

	FHitboxData(const UHitboxDataAsset* InHitboxDataAsset, const UWorld* InWorld, AActor* InOwner, FVector InLocation, uint32 InId)
		: HitboxDataAsset(InHitboxDataAsset), World(InWorld), Owner(InOwner), Location(InLocation), Id(InId)
	{
	}

	friend bool operator==(const FHitboxData& Lhs, const FHitboxData& RHS)
	{
		return Lhs.HitboxDataAsset == RHS.HitboxDataAsset
			&& Lhs.World == RHS.World
			&& Lhs.Owner == RHS.Owner
			&& Lhs.Location == RHS.Location
			&& Lhs.SkeletalMesh == RHS.SkeletalMesh
			&& Lhs.SocketToFollow == RHS.SocketToFollow
			&& Lhs.Id == RHS.Id
			&& Lhs.HitPawnsIds == RHS.HitPawnsIds;
	}

	friend bool operator!=(const FHitboxData& Lhs, const FHitboxData& RHS)
	{
		return !(Lhs == RHS);
	}
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
	void EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, const UWorld* World, AActor* Owner, USkeletalMeshComponent* SkeletalMesh, const FName& SocketToFollow,
	                  uint32 Id);
	void DisableHitbox(uint32 Id);

protected:
	TArray<FHitboxData> ActiveHitboxes;

	virtual void BeginPlay() override;

	void ApplyHitboxData(FHitboxData& HitboxData) const;

private:
	void RemoveHitboxDataById(uint32 Id);
};
