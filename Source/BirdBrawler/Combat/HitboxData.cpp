#include "HitboxData.h"

FHitboxData::FHitboxData(const UHitboxDataAsset* InHitboxDataAsset, bool InForceOpponentFacing, float InDamagePercent, const UWorld* InWorld, AActor* InOwner,
                         USkeletalMeshComponent* InSkeletalMesh, const FName& InSocketToFollow, uint32 InId)
	: HitboxDataAsset(InHitboxDataAsset),
	  ForceOpponentFacing(InForceOpponentFacing),
	  DamagePercent(InDamagePercent),
	  World(InWorld),
	  Owner(InOwner),
	  SkeletalMesh(InSkeletalMesh),
	  SocketToFollow(InSocketToFollow),
	  Id(InId)
{
}

FHitboxData::FHitboxData(const UHitboxDataAsset* InHitboxDataAsset, bool InForceOpponentFacing, float InDamagePercent, const UWorld* InWorld, AActor* InOwner,
                         FVector InLocation, uint32 InId)
	: HitboxDataAsset(InHitboxDataAsset),
	  ForceOpponentFacing(InForceOpponentFacing),
	  DamagePercent(InDamagePercent),
	  World(InWorld),
	  Owner(InOwner),
	  Location(InLocation),
	  Id(InId)
{
}

bool operator==(const FHitboxData& Lhs, const FHitboxData& RHS)
{
	return Lhs.HitboxDataAsset == RHS.HitboxDataAsset
		&& Lhs.World == RHS.World
		&& Lhs.Owner == RHS.Owner
		&& Lhs.Location == RHS.Location
		&& Lhs.SkeletalMesh == RHS.SkeletalMesh
		&& Lhs.SocketToFollow == RHS.SocketToFollow
		&& Lhs.Id == RHS.Id
		&& Lhs.HitActorsIds == RHS.HitActorsIds;
}

bool operator!=(const FHitboxData& Lhs, const FHitboxData& RHS)
{
	return !(Lhs == RHS);
}
