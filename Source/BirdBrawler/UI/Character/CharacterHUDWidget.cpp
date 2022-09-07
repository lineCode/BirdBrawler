#include "CharacterHUDWidget.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "Components/TextBlock.h"

void UCharacterHUDWidget::SetOwner(ABirdBrawlerCharacter* InOwner)
{
	Owner = InOwner;
}

void UCharacterHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// TODO: Do not call this every frame!!!
	Refresh();
}

void UCharacterHUDWidget::Refresh()
{
	if (Owner && DamagePercentText)
	{
		FNumberFormattingOptions Options;
		Options.SetMaximumFractionalDigits(1);

		DamagePercentText->SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), Owner->DamagePercent)));
	}
}
