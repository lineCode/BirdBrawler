#include "CharacterStateBase.h"

#include <string>

#include "Debug.h"

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
	// TODO: ugly af
	if (FSMOwner->IsStateActive(Name))
	{
		const std::string MoveNameStr = TCHAR_TO_UTF8(*(MoveName.ToString()));
		const std::string StateNameStr = TCHAR_TO_UTF8(*(Name.ToString()));

		BB_SLOG(FString::Printf(TEXT("[%hs] Move Ended: %hs"),StateNameStr.c_str(), MoveNameStr.c_str()));
		OnCharacterMoveEnded(MoveName);
	}
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

	// TODO: this should be in Enter_Implementation, fix it
	MoveEndedHandle = Character->MoveEndedDelegate.
	                             AddUObject(this, &UCharacterStateBase::InvokeCharacterMoveEndedEvent);

	Super::Init_Implementation();
}
