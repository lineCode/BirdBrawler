#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUDWidget.generated.h"

class ABirdBrawlerCharacter;

UCLASS()
class BIRDBRAWLER_API UCharacterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwner(ABirdBrawlerCharacter* InOwner);

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DamagePercentText = nullptr;

	UPROPERTY(EditAnywhere)
	FLinearColor NoDamagePercentTextColor;

	UPROPERTY(EditAnywhere)
	FLinearColor MaxDamagePercentTextColor;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	ABirdBrawlerCharacter* Owner = nullptr;

	void Refresh();
};
