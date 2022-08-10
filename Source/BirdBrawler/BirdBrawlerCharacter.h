// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSM.h"
#include "IHittable.h"
#include "MovesBufferComponent.h"
#include "BirdBrawlerCharacter.generated.h"

class UInputBuffer;

#define NO_MOVE FName{""}

DECLARE_MULTICAST_DELEGATE_OneParam(FMoveEnded, FName);

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

protected:
	UPROPERTY(EditAnywhere, Category="FSM")
	UFSM* Fsm;

	float MovementDirection{0.f};
	bool Airborne{false};
	FName CurrentMove{NO_MOVE};

public:
	ABirdBrawlerCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnHit(const FVector& Knockback, const ACharacter& Hitter) override;

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

	void EvaluateHitResult(const FHitResult& HitResult);

	FMoveEnded MoveEndedDelegate;

	FORCEINLINE UMovesBufferComponent* GetMovesBufferComponent() const { return MovesBufferComponent; }
	FORCEINLINE UMovesEffectorComponent* GetMovesEffectorComponent() const { return MovesEffectorComponent; }
};
