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

UENUM(BlueprintType)
enum EAirborneState
{
	Airborne,
	Grounded
};

UENUM(BlueprintType)
enum EMovementState
{
	Moving,
	NotMoving
};

UCLASS()
class BIRDBRAWLER_API UCharacterStateBase : public UStateBase
{
	GENERATED_BODY()

public:
	UCharacterStateBase() = default;

	UFUNCTION(BlueprintNativeEvent, Category="State", DisplayName="On Input Buffered")
	void OnInputBuffered(FName Entry);

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
	TArray<FString> GetBufferedInputs() const;

	UFUNCTION(BlueprintCallable)
	void UseBufferedInput(const FString& Input);

	UFUNCTION(BlueprintCallable)
	bool IsAirborne() const;

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	bool CheckAirborneState(TEnumAsByte<EAirborneState>& Branches);

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	bool CheckMovementState(TEnumAsByte<EMovementState>& Branches);

	virtual void Init_Implementation() override;
};
