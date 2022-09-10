#pragma once

#include "CoreMinimal.h"
#include "BirdBrawlerGameMode.h"
#include "BirdBrawler/UI/HUD/CombatHUD.h"
#include "CombatGameMode.generated.h"

class ABirdBrawlerCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FInitialCountdownTick, int);
DECLARE_MULTICAST_DELEGATE(FInitialCountdownStarted);
DECLARE_MULTICAST_DELEGATE(FInitialCountdownEnded);

UCLASS()
class BIRDBRAWLER_API ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	FInitialCountdownTick InitialCountdownTick;
	FInitialCountdownStarted InitialCountdownStarted;
	FInitialCountdownEnded InitialCountdownEnded;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	int CountdownDuration = 5;

private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	ABirdBrawlerCharacter* Character = nullptr;

	UPROPERTY()
	ACombatHUD* HUD = nullptr;

	FTimerHandle CountdownHandle;
	int CountdownSecondsElapsed = 1;

	void OnEachSecondPassed();
};
