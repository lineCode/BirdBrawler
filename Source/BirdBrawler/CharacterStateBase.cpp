#include "CharacterStateBase.h"

namespace
{
	static const FName FirstState{"First"};
}

void UCharacterStateBase::PlayAnimation(UAnimationAsset* AnimationAsset, const bool Loop)
{
	verify(AnimationAsset);
	verify(SkeletalMesh);

	SkeletalMesh->PlayAnimation(AnimationAsset, Loop);
}

bool UCharacterStateBase::IsInputBuffered(const FString& Input, TEnumAsByte<EInputBufferedOutcome>& Branches)
{
	const auto IsBuffered = Character->GetMovesBufferComponent()->IsInputBuffered(Input);
	Branches = IsBuffered ? Buffered : NotBuffered;

	return IsBuffered;
}

void UCharacterStateBase::MoveCharacterHorizontal(const float Value)
{
	verify(Character);
	Character->MoveHorizontal(Value);
}

void UCharacterStateBase::GoToState(const FName StateName)
{
	FSMOwner->PopAllStatesExcept(FirstState);
	FSMOwner->PushState(StateName);
}

void UCharacterStateBase::Init_Implementation()
{
	Super::Init_Implementation();

	Character = Cast<ABirdBrawlerCharacter>(ActorOwner);
	verify(Character);

	SkeletalMesh = Character->FindComponentByClass<USkeletalMeshComponent>();
	verify(SkeletalMesh);
}
