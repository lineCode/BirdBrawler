#include "CombatGameMode.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "Kismet/GameplayStatics.h"

ACombatGameMode::ACombatGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACombatGameMode::BeginPlay()
{
	AGameModeBase::BeginPlay();

	Character = Cast<ABirdBrawlerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	verify(Character);

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	verify(PlayerController);

	HUD = Cast<ACombatHUD>(PlayerController->GetHUD());
	verify(HUD);

	RemainingMatchSeconds = static_cast<float>(MatchDurationSeconds);
	MatchTimedOutEventSent = false;
	MatchRunning = false;

	HUD->OnGameModeReady();

	Character->DisableInput(PlayerController);

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

		Character->EnableInput(PlayerController);

		InitialCountdownEnded.Broadcast();

		MatchRunning = true;
	}
}
