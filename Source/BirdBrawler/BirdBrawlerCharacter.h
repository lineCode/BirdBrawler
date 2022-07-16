// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSM.h"
#include "MovesBufferComponent.h"
#include "BirdBrawlerCharacter.generated.h"

class UInputBuffer;

UCLASS(config=Game)
class ABirdBrawlerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMovesBufferComponent* MovesBufferComponent;

protected:
	UPROPERTY(EditAnywhere, Category="FSM")
	UFSM* Fsm;

public:
	ABirdBrawlerCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	void MoveHorizontal(float Val);

	UFUNCTION(BlueprintCallable)
	void GoToFsmState(const FName StateName);

	FORCEINLINE UMovesBufferComponent* GetMovesBufferComponent() const { return MovesBufferComponent; }
	FORCEINLINE UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
