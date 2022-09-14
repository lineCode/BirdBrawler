#include "HitboxNotifyState.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "MovesEffectorComponent.h"
#include "BirdBrawler/Debug/Debug.h"
#include "Components/SkeletalMeshComponent.h"

void UHitboxNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (HitboxDataAsset)
	{
		if (const auto* Character = Cast<ABirdBrawlerCharacter>(MeshComp->GetOwner()))
		{
			FHitboxData HitboxData = FHitboxData(
				HitboxDataAsset,
				ForceOpponentFacing,
				DamagePercent,
				IgnoreKnockbackMultiplier,
				HitStunIntensity,
				MeshComp->GetWorld(),
				MeshComp->GetOwner(),
				MeshComp,
				SocketName,
				GetUniqueID()
			);

			Character->GetMovesEffectorComponent()->EnableHitbox(std::move(HitboxData));

			if (HitStunIntensity > .0f)
			{
				BB_SLOG("Notify begin");
			}
		}
	}

	HitPawnsIds.Empty();
}

void UHitboxNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (HitboxDataAsset)
	{
		if (const auto* Character = Cast<ABirdBrawlerCharacter>(MeshComp->GetOwner()))
		{
			Character->GetMovesEffectorComponent()->DisableHitbox(GetUniqueID());

			if (HitStunIntensity > .0f)
			{
				BB_SLOG("Notify end");
			}
		}
	}

	HitPawnsIds.Empty();
}
