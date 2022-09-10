#include "HUDInitialCountdown.h"

#include "Components/TextBlock.h"

void UHUDInitialCountdown::DisplayRemainingSeconds(int RemainingSeconds)
{
	verify(CountdownText);

	CountdownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainingSeconds)));
}
