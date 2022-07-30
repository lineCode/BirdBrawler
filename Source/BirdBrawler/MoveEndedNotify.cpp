#include "MoveEndedNotify.h"

#include "BirdBrawlerCharacter.h"
#include "Debug.h"

void UMoveEndedNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (const auto* Character = Cast<ABirdBrawlerCharacter>(MeshComp->GetOwner()))
	{
		Character->InvokeMoveEndedDelegate(MoveName);

		Debug::ScreenLog("Move ended notify");
	}
	else
	{
		Debug::ScreenLog("Cannot notify move ended");
	}
}
