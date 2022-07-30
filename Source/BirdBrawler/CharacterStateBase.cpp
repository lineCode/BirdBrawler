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

bool UCharacterStateBase::CheckMovementState(TEnumAsByte<EMovementState>& Branches)
{
	Branches = Character->GetMovementDirection() != 0.f ? Moving : NotMoving;
	return Character->GetMovementDirection() != 0.f;
}

void UCharacterStateBase::GoToFsmState(FName StateName) const
{
	Character->GoToFsmState(StateName);
}

void UCharacterStateBase::InvokeCharacterMoveEndedEvent(FName MoveName)
{
	OnCharacterMoveEnded(MoveName);
}

void UCharacterStateBase::OnCharacterMoveEnded_Implementation(FName MoveName)
{
}

void UCharacterStateBase::Init_Implementation()
{
	Character = Cast<ABirdBrawlerCharacter>(ActorOwner);
	verify(Character);

	SkeletalMesh = Character->FindComponentByClass<USkeletalMeshComponent>();
	verify(SkeletalMesh);

	Super::Init_Implementation();
}

void UCharacterStateBase::Enter_Implementation()
{
	MoveEndedHandle = Character->MoveEndedDelegate.
	                             AddUObject(this, &UCharacterStateBase::InvokeCharacterMoveEndedEvent);
	Super::Enter_Implementation();
}

void UCharacterStateBase::Exit_Implementation()
{
	Character->MoveEndedDelegate.Remove(MoveEndedHandle);

	Super::Exit_Implementation();
}
