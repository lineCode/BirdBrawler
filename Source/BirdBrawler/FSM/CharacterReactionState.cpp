// Copyright (c) Giammarco Agazzotti


#include "CharacterReactionState.h"

void UCharacterReactionState::Enter_Implementation()
{
	Super::Enter_Implementation();

	verify(Character);
	Character->EnableAirMovement(false);
}

void UCharacterReactionState::Exit_Implementation()
{
	Super::Exit_Implementation();

	verify(Character);
	Character->EnableAirMovement(true);
}
