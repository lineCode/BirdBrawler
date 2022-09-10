#include "HUDCountdown.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

void UHUDCountdown::DisplayRemainingSeconds(float Seconds)
{
	TimerText->SetText(FText::FromString(UKismetStringLibrary::TimeSecondsToString(Seconds)));
}
