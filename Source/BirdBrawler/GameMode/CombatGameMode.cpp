#include "CombatGameMode.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "Kismet/GameplayStatics.h"

void ACombatGameMode::BeginPlay()
{
	AGameModeBase::BeginPlay();

	Character = Cast<ABirdBrawlerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	verify(Character);

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	verify(PlayerController);

	HUD = Cast<ACombatHUD>(PlayerController->GetHUD());
	verify(HUD);

	HUD->OnGameModeReady();

	Character->DisableInput(PlayerController);

	CountdownSecondsElapsed = 1;
	GetWorldTimerManager().SetTimer(CountdownHandle, this, &ACombatGameMode::OnEachSecondPassed, 1.0f, true, -1);
	InitialCountdownStarted.Broadcast();
}

void ACombatGameMode::Tick(float DeltaSeconds)
{
	AGameModeBase::Tick(DeltaSeconds);
}

void ACombatGameMode::OnEachSecondPassed()
{
	InitialCountdownTick.Broadcast(CountdownDuration - CountdownSecondsElapsed);

	if (++CountdownSecondsElapsed > CountdownDuration)
	{
		GetWorldTimerManager().ClearTimer(CountdownHandle);

		Character->EnableInput(PlayerController);

		InitialCountdownEnded.Broadcast();
	}
}
