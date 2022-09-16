#include "MovesEffectorComponent.h"

#include "CombatUtils.h"
#include "DrawDebugHelpers.h"
#include "IHittable.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "BirdBrawler/Collision/CustomTraceChannels.h"
#include "BirdBrawler/Debug/Debug.h"
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

	HitStunHandler = FCombatUtils::GetHitStunHandler(GetWorld());
	verify(HitStunHandler);
}

void UMovesEffectorComponent::ActivateHitbox(FHitboxData& HitboxData)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceTypes;

	const EObjectTypeQuery TargetCollisionType = UEngineTypes::ConvertToObjectType(TraceChannel_Hurtbox);
	TargetTraceTypes.Add(TargetCollisionType);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(HitboxData.Owner);

	const bool HasSocketToFollow = !HitboxData.SocketToFollow.ToString().IsEmpty();
	const FVector Location = HasSocketToFollow ? HitboxData.SkeletalMesh->GetSocketLocation(HitboxData.SocketToFollow) : HitboxData.Location;

	FHitResult OutHitResult;
	const bool SuccessfulHit = UKismetSystemLibrary::SphereTraceSingleForObjects(HitboxData.World, Location, Location,
	                                                                             HitboxData.HitboxDataAsset->Radius, TargetTraceTypes,
	                                                                             false, ActorsToIgnore, EDrawDebugTrace::None, OutHitResult, true);

	if (SuccessfulHit)
	{
		if (!CharacterWasHitPreviously(OutHitResult.Actor->GetUniqueID(), HitboxData.Id))
		{
			RegisterHitCharacter(OutHitResult.Actor->GetUniqueID(), HitboxData.Id);

			// TODO: must work on non-characters too.
			// TODO: this logic works but it's redundant, a refactor is needed.
			if (auto* HitCharacter = Cast<ABirdBrawlerCharacter>(OutHitResult.Actor))
			{
				if (HitCharacter->Invincible)
				{
					if (HitCharacter->InvincibleAllowDamage)
					{
						HitCharacter->DamagePercent += HitboxData.DamagePercent;

						HitStunHandler->ApplyHitStun(Character, HitboxData.HitStunDuration);
						HitStunHandler->ApplyHitStun(HitCharacter, HitboxData.HitStunDuration, HitboxData.Shake);
					}
				}
				else
				{
					HitCharacter->DamagePercent += HitboxData.DamagePercent;

					const FVector KnockbackVector = CalculateKnockbackVector(HitboxData.HitboxDataAsset);
					if (auto* Hittable = Cast<IHittable>(HitCharacter))
					{
						Hittable->OnHit(KnockbackVector, HitboxData.Owner);
					}

					FCombatUtils::ApplyKnockbackTo(KnockbackVector, HitboxData.HitboxDataAsset->KnockbackForce, HitCharacter, HitboxData.IgnoreKnockbackMultiplier);

					if (HitboxData.ForceOpponentFacing)
					{
						FCombatUtils::FaceTargetCharacter(Character, HitCharacter);
					}

					HitStunHandler->ApplyHitStun(Character, HitboxData.HitStunDuration);
					HitStunHandler->ApplyHitStun(HitCharacter, HitboxData.HitStunDuration, HitboxData.Shake);
				}
			}
		}
	}
}

bool UMovesEffectorComponent::CharacterWasHitPreviously(uint32 CharacterUniqueId, uint32 HitBoxId)
{
	HitboxActorPair* CurrentPair = HitboxHitActorPairs.FindByPredicate([&](const HitboxActorPair& Pair) { return Pair.Key == HitBoxId; });
	if (CurrentPair && CurrentPair->Value.Contains(CharacterUniqueId))
	{
		return true;
	}

	return false;
}

void UMovesEffectorComponent::RegisterHitCharacter(uint32 CharacterUniqueId, uint32 HitBoxId)
{
	HitboxActorPair* CurrentPair = HitboxHitActorPairs.FindByPredicate([&](const HitboxActorPair& Pair) { return Pair.Key == HitBoxId; });
	if (CurrentPair)
	{
		CurrentPair->Value.Emplace(CharacterUniqueId);
	}
	else
	{
		HitboxHitActorPairs.Emplace(HitboxActorPair(HitBoxId, TArray<uint32>{CharacterUniqueId}));
	}
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

	for (int i = HitboxHitActorPairs.Num() - 1; i >= 0; --i)
	{
		if (HitboxHitActorPairs[i].Key == Id)
		{
			// TODO: this avoids re-allocating memory. Could this become too big?
			HitboxHitActorPairs[i].Value.Empty();
			break;
		}
	}
}

void UMovesEffectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FHitboxData& HitboxData : ActiveHitboxes)
	{
		ActivateHitbox(HitboxData);
	}
}

void UMovesEffectorComponent::EnableHitbox(FHitboxData&& HitboxData)
{
	if (!ActiveHitboxes.ContainsByPredicate([&](const FHitboxData& Data) { return HitboxData.Id == Data.Id; }))
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
