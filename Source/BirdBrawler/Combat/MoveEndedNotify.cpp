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
			Character->SetCurrentMove(NO_MOVE);
		}

		const std::string MoveNameStr = TCHAR_TO_UTF8(*(MoveName.ToString()));
		//BB_SLOG_WARN(FString::Printf(TEXT("Move Ended: %hs"), MoveNameStr.c_str()));
	}
	else
	{
		BB_SLOG_ERR("Cannot notify move ended");
	}
}
