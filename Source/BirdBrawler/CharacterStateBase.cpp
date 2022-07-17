#include "CharacterStateBase.h"

namespace
{
	static const FName FirstState{"First"};
}

void UCharacterStateBase::OnInputBuffered_Implementation(FName Entry)
{
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

TArray<FString> UCharacterStateBase::GetBufferedInputs() const
{
	return Character->GetMovesBufferComponent()->GetBufferedInputs();
}

void UCharacterStateBase::UseBufferedInput(const FString& Input)
{
	Character->GetMovesBufferComponent()->UseBufferedInput(Input);
}

bool UCharacterStateBase::IsAirborne() const
{
	return Character->IsAirborne();
}

bool UCharacterStateBase::CheckAirborneState(TEnumAsByte<EAirborneState>& Branches)
{
	Branches = Character->IsAirborne() ? Airborne : Grounded;
	return Character->IsAirborne();
}

void UCharacterStateBase::Init_Implementation()
{
	Super::Init_Implementation();

	Character = Cast<ABirdBrawlerCharacter>(ActorOwner);
	verify(Character);

	SkeletalMesh = Character->FindComponentByClass<USkeletalMeshComponent>();
	verify(SkeletalMesh);
}
