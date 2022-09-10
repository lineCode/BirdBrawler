#include "CombatHUD.h"

#include "HUDCountdown.h"
#include "HUDInitialCountdown.h"
#include "BirdBrawler/GameMode/CombatGameMode.h"
#include "BirdBrawler/UI/UIUtils.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ACombatHUD::OnGameModeReady()
{
	// Having the game mode reference the HUD and the HUD reference the game mode is not good... I'll find a solution eventually
	GameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	verify(GameMode);

	CountdownWidget = CreateWidget<UHUDCountdown>(GetOwningPlayerController(), CountdownWidgetType, TEXT("Countdown"));
	InitialCountdownWidget = CreateWidget<UHUDInitialCountdown>(GetOwningPlayerController(), InitialCountdownWidgetType, TEXT("InitialCountdown"));

	InitialCountdownTickHandle = GameMode->InitialCountdownTick.AddUObject(this, &ACombatHUD::OnInitialCountdownTick);
	InitialCountdownStartedHandle = GameMode->InitialCountdownStarted.AddUObject(this, &ACombatHUD::OnInitialCountdownStarted);
	InitialCountdownEndedHandle = GameMode->InitialCountdownEnded.AddUObject(this, &ACombatHUD::OnInitialCountdownEnded);
	MatchCountdownTickHandle = GameMode->MatchCountdownTick.AddUObject(this, &ACombatHUD::OnMatchCountdownTick);
}

void ACombatHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GameMode->InitialCountdownTick.Remove(InitialCountdownTickHandle);
	GameMode->InitialCountdownStarted.Remove(InitialCountdownStartedHandle);
	GameMode->InitialCountdownEnded.Remove(InitialCountdownEndedHandle);
	GameMode->MatchCountdownTick.Remove(MatchCountdownTickHandle);

	Super::EndPlay(EndPlayReason);
}

void ACombatHUD::ShowWidget(UUserWidget* Widget)
{
	UIUtils::ShowWidget(Widget);
}

void ACombatHUD::HideWidget(UUserWidget* Widget)
{
	UIUtils::HideWidget(Widget);
}

void ACombatHUD::OnInitialCountdownTick(int RemainingSeconds)
{
	verify(InitialCountdownWidget);

	InitialCountdownWidget->DisplayRemainingSeconds(RemainingSeconds);
}

void ACombatHUD::OnInitialCountdownStarted()
{
	verify(InitialCountdownWidget);
	UIUtils::ShowWidget(InitialCountdownWidget);
}

void ACombatHUD::OnInitialCountdownEnded()
{
	verify(InitialCountdownWidget);
	UIUtils::HideWidget(InitialCountdownWidget);

	UIUtils::ShowWidget(CountdownWidget);
}

void ACombatHUD::OnMatchCountdownTick(float RemainingSeconds)
{
	verify(CountdownWidget);

	CountdownWidget->DisplayRemainingSeconds(RemainingSeconds);
}
