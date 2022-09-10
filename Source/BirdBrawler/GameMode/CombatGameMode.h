#pragma once

#include "CoreMinimal.h"
#include "BirdBrawlerGameMode.h"
#include "BirdBrawler/UI/HUD/CombatHUD.h"
#include "CombatGameMode.generated.h"

class ABirdBrawlerCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FInitialCountdownTick, int);
DECLARE_MULTICAST_DELEGATE(FInitialCountdownStarted);
DECLARE_MULTICAST_DELEGATE(FInitialCountdownEnded);
DECLARE_MULTICAST_DELEGATE_OneParam(FMatchCountdownTick, float);
DECLARE_MULTICAST_DELEGATE(FMatchTimedOut);

UCLASS()
class BIRDBRAWLER_API ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACombatGameMode();
	
	FInitialCountdownTick InitialCountdownTick;
	FInitialCountdownStarted InitialCountdownStarted;
	FInitialCountdownEnded InitialCountdownEnded;
	FMatchCountdownTick MatchCountdownTick;
	FMatchTimedOut MatchTimedOut;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	int CountdownDurationSeconds = 5;

	UPROPERTY(EditAnywhere)
	int MatchDurationSeconds = 120;

private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	ABirdBrawlerCharacter* Character = nullptr;

	UPROPERTY()
	ACombatHUD* HUD = nullptr;

	FTimerHandle CountdownHandle;
	int CountdownSecondsElapsed = 1;

	float RemainingMatchSeconds;
	bool MatchTimedOutEventSent = false;

	bool MatchRunning = false;

	void OnEachSecondPassed();
};
