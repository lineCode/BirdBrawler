#include "MovesEffectorComponent.h"

#include "CombatUtils.h"
#include "DrawDebugHelpers.h"
#include "IHittable.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UMovesEffectorComponent::UMovesEffectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMovesEffectorComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ABirdBrawlerCharacter>(GetOwner());
	verify(Character);
}

void UMovesEffectorComponent::ApplyHitboxData(FHitboxData& HitboxData) const
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceTypes;

	// TODO: filter by "hittable character" not by pawn
	// TODO: I think this queries the character's capsule. implement proper hurtboxes
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

	const FVector KnockbackVector = CalculateKnockbackVector(HitboxData.HitboxDataAsset);

	const FVector EndPoint = Location + KnockbackVector * 50.f;

	if (DidHit && !HitboxData.HitActorsIds.Contains(OutHit.Actor->GetUniqueID()))
	{
		// TODO: must work on non-characters too.
		// TODO: this logic works but it's redundant, a refactor is needed.
		if (auto* HitCharacter = Cast<ABirdBrawlerCharacter>(OutHit.Actor))
		{
			if (HitCharacter->Invincible)
			{
				if (HitCharacter->InvincibleAllowDamage)
				{
					HitCharacter->DamagePercent += HitboxData.DamagePercent;
				}
			}
			else
			{
				HitboxData.HitActorsIds.Emplace(OutHit.Actor->GetUniqueID());

				HitCharacter->DamagePercent += HitboxData.DamagePercent;

				if (auto* Hittable = Cast<IHittable>(HitCharacter))
				{
					Hittable->OnHit(KnockbackVector, HitboxData.Owner);
				}

				FCombatUtils::ApplyKnockbackTo(KnockbackVector, HitboxData.HitboxDataAsset->KnockbackForce, HitCharacter, HitboxData.IgnoreKnockbackMultiplier);

				if (HitboxData.ForceOpponentFacing)
				{
					FCombatUtils::FaceTargetCharacter(Character, HitCharacter);
				}
			}
		}
	}

	DrawDebugDirectionalArrow(HitboxData.World, Location, EndPoint, 10.f, FColor::Red, false, 5);
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

void UMovesEffectorComponent::EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, bool ForceOpponentFacing, float DamagePercent, bool IgnoreKnockbackMultiplier,
                                           const UWorld* World,
                                           AActor*
                                           Owner,
                                           const FVector& Location,
                                           uint32 Id)
{
	const FHitboxData HitboxData = FHitboxData(HitboxDataAsset, ForceOpponentFacing, DamagePercent, IgnoreKnockbackMultiplier, World, Owner, Location, Id);

	if (!ActiveHitboxes.Contains(HitboxData))
	{
		ActiveHitboxes.Add(HitboxData);
	}
}

void UMovesEffectorComponent::EnableHitbox(const UHitboxDataAsset* HitboxDataAsset, bool ForceOpponentFacing, float DamagePercent, bool IgnoreKnockbackMultiplier,
                                           const UWorld* World,
                                           AActor*
                                           Owner,
                                           USkeletalMeshComponent* SkeletalMesh,
                                           const FName& SocketToFollow, uint32 Id)
{
	const FHitboxData HitboxData = FHitboxData(HitboxDataAsset, ForceOpponentFacing, DamagePercent, IgnoreKnockbackMultiplier, World, Owner, SkeletalMesh, SocketToFollow,
	                                           Id);

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

FVector UMovesEffectorComponent::CalculateKnockbackVector(const UHitboxDataAsset* HitboxDataAsset) const
{
	FVector Forward = Character->GetActorForwardVector();

	bool FacingRight = Character->IsFacingRight();
	float FinalKnockbackOrientation = FacingRight ? HitboxDataAsset->KnockbackOrientation : -HitboxDataAsset->KnockbackOrientation;

	FRotator Rotator = FRotator(0, 0, FinalKnockbackOrientation);
	return Rotator.RotateVector(Forward);
}
