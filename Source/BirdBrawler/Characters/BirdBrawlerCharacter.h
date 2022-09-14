// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSM.h"
#include "BirdBrawler/Combat/IHittable.h"
#include "BirdBrawler/Input/MovesBufferComponent.h"
#include "BirdBrawlerCharacter.generated.h"

class UInputBuffer;

UENUM()
enum class EPushDirection : uint8
{
	Front,
	Back
};

#define NO_MOVE FName{""}

DECLARE_MULTICAST_DELEGATE_OneParam(FMoveEnded, FName);
DECLARE_MULTICAST_DELEGATE_OneParam(FCurrentMoveChanged, FName);

UCLASS(config=Game)
class ABirdBrawlerCharacter : public ACharacter, public IHittable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMovesBufferComponent* MovesBufferComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UNavMovementComponent* NavMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMovesEffectorComponent* MovesEffectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMeshComponent;

public:
	// TODO: a lot of this members could go to separate components
	bool Invincible = false;
	bool InvincibleAllowDamage = false;

	float DamagePercent = 0.f;

	ABirdBrawlerCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnHit(const FVector& Knockback, AActor* Hitter) override;

	void MoveHorizontal(float Val);

	UFUNCTION(BlueprintCallable)
	void GoToFsmState(const FName StateName);

	UFUNCTION(BlueprintCallable)
	float GetMovementDirection() const;

	UFUNCTION(BlueprintCallable)
	void SetMovementDirection(float Direction);

	UFUNCTION(BlueprintCallable)
	float GetMovementRotationYaw() const;

	UFUNCTION(BlueprintCallable)
	bool IsAirborne() const;

	UFUNCTION(BlueprintCallable)
	FName GetCurrentMove() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentMove(FName MoveName);

	UFUNCTION(BlueprintCallable)
	float GetInputMovement() const;

	UFUNCTION(BlueprintCallable)
	void InvokeMoveEndedDelegate(FName MoveName) const;

	UFUNCTION(BlueprintCallable)
	void InvokeCurrentMoveChangedDelegate(FName MoveName) const;

	UFUNCTION(BlueprintCallable)
	void PlayAnimation(UAnimationAsset* AnimationAsset, bool Loop = false) const;

	UFUNCTION(BlueprintCallable)
	void PlayAnimationSequence(UAnimSequenceBase* AnimationSequence, bool Loop = false) const;

	UFUNCTION(BlueprintCallable)
	void Push(EPushDirection Direction, float Intensity);

	UFUNCTION(BlueprintCallable)
	bool IsFacingRight() const;

	UFUNCTION(BlueprintCallable)
	void SetInvincible(bool InInvincible, bool InAllowDamage);

	UFUNCTION(BlueprintCallable)
	void ApplyHitStun(float Intensity);

	float GetKnockbackMultiplier() const;

	FMoveEnded MoveEndedDelegate;
	FCurrentMoveChanged CurrentMoveChangedDelegate;

	FORCEINLINE UMovesBufferComponent* GetMovesBufferComponent() const { return MovesBufferComponent; }
	FORCEINLINE UMovesEffectorComponent* GetMovesEffectorComponent() const { return MovesEffectorComponent; }
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent; }

protected:
	UPROPERTY(EditAnywhere, Category="FSM")
	UFSM* Fsm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InvincibilityMaterialPulseFrequency = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InvincibilityMaterialPulseIntensity = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve KnockbackMultiplierCurve;

	UPROPERTY()
	class UWidgetComponent* CharacterHUDWidget = nullptr;

	float MovementDirection = 0.f;
	bool Airborne = false;
	FName CurrentMove = NO_MOVE;

private:
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> InitialMaterialInstances;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> EditableMaterialInstances;

	FTimerHandle HitStunTimerHandle;

	void InitFsm();
	void InitMaterialInstances();
	void InitHUD();

	void SetMaterials(TArray<UMaterialInstanceDynamic*>& Materials);
	void SetInvincibilityMaterialsParameters();

	void OnHitStunTimerEnded();
};
