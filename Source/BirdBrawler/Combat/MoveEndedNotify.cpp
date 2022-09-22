#include "MoveEndedNotify.h"

#include <string>

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "BirdBrawler/Debug/Debug.h"

void UMoveEndedNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (auto* Character = Cast<ABirdBrawlerCharacter>(MeshComp->GetOwner()))
	{
		Character->InvokeMoveEndedDelegate(MoveName);

		if (ResetMoveAfterNotify)
		{
			Character->SetCurrentMove(MOVE_NONE);
		}
	}
	else
	{
		BB_SLOG_ERR("Cannot notify move ended");
	}
}
