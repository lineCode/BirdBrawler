#include "MovesEffectorComponent.h"

#include "DrawDebugHelpers.h"
#include "IHittable.h"
#include "Kismet/KismetSystemLibrary.h"

UMovesEffectorComponent::UMovesEffectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovesEffectorComponent::ApplyHitboxData(FHitboxData& HitboxData) const
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceTypes;

	// TODO: filter by "hittable character" not by pawn
	const EObjectTypeQuery PawnCollisionType = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	TargetTraceTypes.Add(PawnCollisionType);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(HitboxData.Owner);

	const FVector Location = HitboxData.SocketToFollow.ToString().IsEmpty() ? HitboxData.Location : HitboxData.SkeletalMesh->GetSocketLocation(HitboxData.SocketToFollow);

	FHitResult OutHit;
	const bool DidHit = UKismetSystemLibrary::SphereTraceSingleForObjects(HitboxData.World, Location, Location,
	                                                                      HitboxData.HitboxDataAsset->Radius, TargetTraceTypes,
	                                                                      false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	DrawDebugSphere(HitboxData.World, Location, HitboxData.HitboxDataAsset->Radius, 7, FColor::Red, false);

	const FVector KnockbackVector = GetKnockbackVector(HitboxData.HitboxDataAsset);

	const FVector EndPoint = Location + KnockbackVector * 50.f;

	if (DidHit && !HitboxData.HitPawnsIds.Contains(OutHit.Actor->GetUniqueID()))
	{
		HitboxData.HitPawnsIds.Emplace(OutHit.Actor->GetUniqueID());

		if (auto* Hittable = Cast<IHittable>(OutHit.Actor))
		{
			// TODO: passing owner by value is expensive
			Hittable->OnHit(KnockbackVector, *HitboxData.Owner);
		}
	}

	DrawDebugDirectionalArrow(HitboxData.World, Location, EndPoint, 10.f, FColor::Red, false);
}

void UMovesEffectorComponent::RemoveHitboxDataById(const uint32 Id)
{
	for (int i = ActiveHitboxes.Num() - 1; i >= 0; --i)
	{
		if (ActiveHitboxes[i].Id == Id)
		{
			ActiveHitboxes.RemoveAt(i);
			break;
		}
	}
}

void UMovesEffectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FHitboxData& HitboxData : ActiveHitboxes)
	{
		ApplyHitboxData(HitboxData);
	}
}

void UMovesEffectorComponent::EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, const UWorld* World, AActor* Owner, const FVector& Location, uint32 Id)
{
	const FHitboxData HitboxData = FHitboxData(HitboxDataAsset, World, Owner, Location, Id);

	if (!ActiveHitboxes.Contains(HitboxData))
	{
		ActiveHitboxes.Add(HitboxData);
	}
}

void UMovesEffectorComponent::EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, const UWorld* World, AActor* Owner, USkeletalMeshComponent* SkeletalMesh,
                                           const FName& SocketToFollow, uint32 Id)
{
	const FHitboxData HitboxData = FHitboxData(HitboxDataAsset, World, Owner, SkeletalMesh, SocketToFollow, Id);

	if (!ActiveHitboxes.Contains(HitboxData))
	{
		ActiveHitboxes.Add(HitboxData);
	}
}

void UMovesEffectorComponent::DisableHitbox(const uint32 Id)
{
	RemoveHitboxDataById(Id);
}

FHitboxData* UMovesEffectorComponent::GetHitboxData(const uint32 Id)
{
	return ActiveHitboxes.FindByPredicate([&](const FHitboxData& HitboxData) { return HitboxData.Id == Id; });
}

FVector UMovesEffectorComponent::GetKnockbackVector(const UHitboxDataAsset* HitboxDataAsset)
{
	const FRotator Rotator = FRotator(HitboxDataAsset->KnockbackOrientation, 90.f, 0.f);
	return Rotator.RotateVector(FVector(1.f, 0.f, 0.f));
}
