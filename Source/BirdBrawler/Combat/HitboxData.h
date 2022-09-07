#pragma once

class UHitboxDataAsset;

struct FHitboxData
{
	const UHitboxDataAsset* HitboxDataAsset = nullptr;
	const bool ForceOpponentFacing = true;
	const float DamagePercent = 0.f;
	bool IgnoreKnockbackMultiplier = false;
	const UWorld* World = nullptr;
	AActor* Owner = nullptr;
	FVector Location;
	USkeletalMeshComponent* SkeletalMesh = nullptr;
	FName SocketToFollow = "";
	uint32 Id;

	// TODO: move this to MovesEffectorComponent
	TArray<uint32> HitActorsIds;

	FHitboxData(const UHitboxDataAsset* InHitboxDataAsset, bool InForceOpponentFacing, float InDamagePercent, bool InIgnoreKnockbackMultiplier, const UWorld* InWorld,
	            AActor* InOwner, USkeletalMeshComponent* InSkeletalMesh, const FName& InSocketToFollow, uint32 InId);

	FHitboxData(const UHitboxDataAsset* InHitboxDataAsset, bool InForceOpponentFacing, float InDamagePercent, bool InIgnoreKnockbackMultiplier, const UWorld* InWorld,
	            AActor* InOwner, FVector InLocation, uint32 InId);

	friend bool operator==(const FHitboxData& Lhs, const FHitboxData& RHS);
	friend bool operator!=(const FHitboxData& Lhs, const FHitboxData& RHS);
};
