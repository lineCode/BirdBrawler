#pragma once

#include "CoreMinimal.h"
#include "BirdBrawlerCharacter.h"
#include "StateBase.h"
#include "CharacterStateBase.generated.h"

UENUM(BlueprintType)
enum EInputBufferedOutcome
{
	Buffered,
	NotBuffered
};

UCLASS()
class BIRDBRAWLER_API UCharacterStateBase : public UStateBase
{
	GENERATED_BODY()

public:
	UCharacterStateBase() = default;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ABirdBrawlerCharacter* Character{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh{nullptr};

	UFUNCTION(BlueprintCallable)
	void PlayAnimation(UAnimationAsset* AnimationAsset, const bool Loop);

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	bool IsInputBuffered(const FString& Input, TEnumAsByte<EInputBufferedOutcome>& Branches);

	UFUNCTION(BlueprintCallable)
	void MoveCharacterHorizontal(const float Value);

	UFUNCTION(BlueprintCallable)
	void GoToState(const FName StateName);

	virtual void Init_Implementation() override;
};
