#include "CombatGameMode.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"

namespace
{
	constexpr auto MaxCharacters = 4;
}

ACombatGameMode::ACombatGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACombatGameMode::BeginPlay()
{
	AGameModeBase::BeginPlay();

	// TODO: For testing purposes only [BEGIN]
	CharactersTypesToSpawn.Emplace(DefaultPawnClass);
	// TODO: For testing purposes only [END]

	verify(CharactersTypesToSpawn.Num() <= MaxCharacters);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	verify(PlayerStarts.Num() == MaxCharacters);

	// TODO: Assume characters to spawn are correctly ordered at this point
	for (int i = 0; i < CharactersTypesToSpawn.Num(); ++i)
	{
		auto* Player = UGameplayStatics::CreatePlayer(GetWorld());
		Players.Emplace(Player);
	}

	//EnablePlayersInput(false);

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	verify(PlayerController);

	// Having the game mode reference the HUD and the HUD reference the game mode is not good... I'll find a solution eventually
	HUD = Cast<ACombatHUD>(PlayerController->GetHUD());
	verify(HUD);

	RemainingMatchSeconds = static_cast<float>(MatchDurationSeconds);
	MatchTimedOutEventSent = false;
	MatchRunning = false;

	HUD->OnGameModeReady();

	CountdownSecondsElapsed = 1;
	GetWorldTimerManager().SetTimer(CountdownHandle, this, &ACombatGameMode::OnEachSecondPassed, 1.0f, true, -1);
	InitialCountdownStarted.Broadcast();
}

void ACombatGameMode::Tick(float DeltaSeconds)
{
	AGameModeBase::Tick(DeltaSeconds);

	if (MatchRunning)
	{
		if (!MatchTimedOutEventSent)
		{
			RemainingMatchSeconds -= DeltaSeconds;
			MatchCountdownTick.Broadcast(RemainingMatchSeconds);

			if (RemainingMatchSeconds <= 0.f)
			{
				MatchTimedOutEventSent = true;
				MatchTimedOut.Broadcast();
			}
		}
	}
}

void ACombatGameMode::OnEachSecondPassed()
{
	InitialCountdownTick.Broadcast(CountdownDurationSeconds - CountdownSecondsElapsed);

	if (++CountdownSecondsElapsed > CountdownDurationSeconds)
	{
		GetWorldTimerManager().ClearTimer(CountdownHandle);

		//EnablePlayersInput(true);

		InitialCountdownEnded.Broadcast();

		MatchRunning = true;
	}
}

void ACombatGameMode::EnablePlayersInput(bool Enable)
{
	verify(Players.Num() == SpawnedCharacters.Num());
	int Count = SpawnedCharacters.Num();

	for (int i = 0; i < Count; ++i)
	{
		if (Enable)
		{
			SpawnedCharacters[i]->EnableInput(Players[i]);
		}
		else
		{
			SpawnedCharacters[i]->DisableInput(Players[i]);
		}
	}
}
