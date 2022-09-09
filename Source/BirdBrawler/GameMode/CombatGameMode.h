#pragma once

#include "CoreMinimal.h"
#include "BirdBrawlerGameMode.h"
#include "CombatGameMode.generated.h"

class ABirdBrawlerCharacter;

UCLASS()
class BIRDBRAWLER_API ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
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

	FTimerHandle CountdownHandle;
	int CountdownSecondsElapsed = 0;

	void OnEachSecondPassed();
};
