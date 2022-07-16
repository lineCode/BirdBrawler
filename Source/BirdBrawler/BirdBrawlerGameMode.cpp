// Copyright Epic Games, Inc. All Rights Reserved.

#include "BirdBrawlerGameMode.h"
#include "BirdBrawlerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABirdBrawlerGameMode::ABirdBrawlerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
