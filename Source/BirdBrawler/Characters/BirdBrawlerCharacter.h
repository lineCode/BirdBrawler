// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSM.h"
#include "BirdBrawler/Combat/IHittable.h"
#include "BirdBrawler/Input/MovesBufferComponent.h"
#include "Components/BoxComponent.h"
#include "BirdBrawlerCharacter.generated.h"

class UInputBuffer;

UENUM()
enum class EPushDirection : uint8
{
	Front,
	Back
};

USTRUCT()
struct FWallCollisionInfo
{
	GENERATED_BODY()

	bool DidCollide = false;

	// TODO: Maybe add an array of walls in the wall collision info and not just a single wall?
	UPROPERTY()
	AActor* Wall = nullptr;
};

#define MOVE_NONE FName("")

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

	virtual void OnHit(const FVector& Knockback, float PitchDegreesAbs, float KnockbackForce, AActor* Hitter) override;

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
	void EnableHitStun(bool Shake);

	UFUNCTION(BlueprintCallable)
	void DisableHitStun();

	UFUNCTION(BlueprintCallable)
	void ApplyAirFrictionIncrease(float ElapsedTimeSinceAirborne);

	float GetKnockbackMultiplier() const;

	bool IsAgainstWall(FWallCollisionInfo& OutWallCollisionInfo) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnWallCollision(FVector CollisionNormal);

	void EnableAirMovement(bool Enable);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve LaunchAirFrictionIncreaseCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshShakeAmplitude = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshShakeFrequency = 60.f;

	UPROPERTY(EditAnywhere)
	float PushboxShiftRatePerFrame = 0.3f;

	UPROPERTY()
	class UWidgetComponent* CharacterHUDWidget = nullptr;

	bool ShakeMesh = false;

	float MovementDirection = 0.f;
	bool Airborne = false;
	FName CurrentMove = MOVE_NONE;

	TArray<float> TimeDilations;

private:
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> InitialMaterialInstances;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> EditableMaterialInstances;

	UPROPERTY()
	UBoxComponent* PushBox = nullptr;

	UPROPERTY()
	UBoxComponent* WallBox = nullptr;

	UPROPERTY()
	TArray<UPrimitiveComponent*> CollidingWalls;

	float CachedAirControl = 0.f;

	void InitFsm();
	void InitMaterialInstances();
	void InitHUD();
	void InitTimeDilations();
	void InitPushBox();
	void InitWallBox();

	void SetMaterials(TArray<UMaterialInstanceDynamic*>& Materials);
	void SetInvincibilityMaterialsParameters();

	void OnHitStunTimerEnded();

	void UpdatePushboxOverlap(float DeltaTime);
	void UpdateWallsOverlap();
};
