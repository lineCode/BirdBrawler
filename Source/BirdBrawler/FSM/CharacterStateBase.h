#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
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

	UFUNCTION(BlueprintNativeEvent, Category="State", DisplayName="On Move Ended")
	void OnCharacterMoveEnded(FName MoveName);

protected:
	bool StateActive = false;
	float ElapsedTimeSinceEnter = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ABirdBrawlerCharacter* Character{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh{nullptr};

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	bool IsInputBuffered(const FString& Input, TEnumAsByte<EInputBufferedOutcome>& Branches);

	UFUNCTION(BlueprintCallable)
	void MoveCharacterHorizontal(const float Value);

	UFUNCTION(BlueprintCallable)
	void UpdateHorizontalMovement();

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

	UFUNCTION(BlueprintCallable)
	void GoToFsmState(FName StateName) const;

	UFUNCTION(BlueprintCallable)
	void PlayAnimation(UAnimationAsset* AnimationAsset, bool Loop = false) const;

	UFUNCTION(BlueprintCallable)
	void PlayAnimationSequence(UAnimSequenceBase* AnimationSequence, bool Loop = false) const;

	UFUNCTION(BlueprintCallable)
	void FaceMovement();

	UFUNCTION(BlueprintPure)
	float GetElapsedTimeSinceStateEnter();

	UFUNCTION(BlueprintCallable)
	void ClearMovesBuffer();

	void InvokeCharacterMoveEndedEvent(FName MoveName);

	virtual void Init_Implementation() override;
	virtual void Enter_Implementation() override;
	virtual void Exit_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;

private:
	FDelegateHandle MoveEndedDelegateHandle;
};
