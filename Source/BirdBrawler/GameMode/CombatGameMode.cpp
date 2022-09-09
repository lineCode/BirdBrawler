#include "CombatGameMode.h"

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "BirdBrawler/Debug/Debug.h"
#include "Kismet/GameplayStatics.h"

void ACombatGameMode::BeginPlay()
{
	AGameModeBase::BeginPlay();

	Character = Cast<ABirdBrawlerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	verify(Character);

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	verify(PlayerController);

	Character->DisableInput(PlayerController);

	CountdownSecondsElapsed = 0;
	GetWorldTimerManager().SetTimer(CountdownHandle, this, &ACombatGameMode::OnEachSecondPassed, 1.0f, true, -1);
}

void ACombatGameMode::Tick(float DeltaSeconds)
{
	AGameModeBase::Tick(DeltaSeconds);
}

void ACombatGameMode::OnEachSecondPassed()
{
	BB_SLOG(FString::Printf(TEXT("Countdown %d"), CountdownSecondsElapsed));
	if (++CountdownSecondsElapsed > CountdownDuration)
	{
		GetWorldTimerManager().ClearTimer(CountdownHandle);

		Character->EnableInput(PlayerController);
	}
}
