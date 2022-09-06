#include "HitboxNotifyState.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "MovesEffectorComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UHitboxNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (HitboxDataAsset)
	{
		if (const auto* Character = Cast<ABirdBrawlerCharacter>(MeshComp->GetOwner()))
		{
			Character->GetMovesEffectorComponent()->EnableHitbox(HitboxDataAsset, ForceOpponentFacing, MeshComp->GetWorld(), MeshComp->GetOwner(), MeshComp, SocketName,
			                                                     GetUniqueID());
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
		}
	}

	HitPawnsIds.Empty();
}
