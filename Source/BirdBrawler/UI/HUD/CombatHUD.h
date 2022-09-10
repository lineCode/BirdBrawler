#pragma once

#include "CoreMinimal.h"
#include "HUDCountdown.h"
#include "GameFramework/HUD.h"
#include "CombatHUD.generated.h"

class ACombatGameMode;

UCLASS()
class BIRDBRAWLER_API ACombatHUD : public AHUD
{
	GENERATED_BODY()

public:
	void OnGameModeReady();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHUDCountdown> CountdownWidgetType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHUDInitialCountdown> InitialCountdownWidgetType;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	ACombatGameMode* GameMode = nullptr;

	UPROPERTY()
	UHUDCountdown* CountdownWidget = nullptr;

	UPROPERTY()
	UHUDInitialCountdown* InitialCountdownWidget = nullptr;

	FDelegateHandle InitialCountdownTickHandle;
	FDelegateHandle InitialCountdownStartedHandle;
	FDelegateHandle InitialCountdownEndedHandle;

	void OnInitialCountdownTick(int RemainingSeconds);
	void OnInitialCountdownStarted();
	void OnInitialCountdownEnded();

	UFUNCTION(BlueprintCallable)
	void ShowWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void HideWidget(UUserWidget* Widget);
};
