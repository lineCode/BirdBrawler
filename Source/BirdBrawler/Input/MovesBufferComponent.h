#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include <deque>

#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "MovesBufferComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BIRDBRAWLER_API UMovesBufferComponent : public UActorComponent
{
	GENERATED_BODY()

	struct FInputBufferEntry
	{
		FString Name;
		bool Used;
	};

public:
	UMovesBufferComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void UseBufferedInput(const FString& Input);

	bool IsInputBuffered(const FString& Input, bool ConsumeEntry = true);

	TArray<FString> GetBufferedInputs() const;

	void ClearBuffer();

	float InputMovement{0.f};

	bool MovingRight{false};
	bool MovingLeft{false};

protected:
	UPROPERTY(EditAnywhere)
	int BufferSizeFrames{6};

	UPROPERTY(EditAnywhere)
	float BufferFrameLength{.32f};

	std::deque<FInputBufferEntry> Buffer;
	float ElapsedFrameTime = 0.f;
	bool BufferChanged{false};

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AddMoveToBuffer(const FString& MoveName);
	bool BufferContainsConsumableInput(const FString& Input) const;


	void OnMoveHorizontal(float Value);

	void OnStartJump();
	void OnStopJump();
	void OnStartMoveRight();
	void OnStopMoveRight();
	void OnStartMoveLeft();
	void OnStopMoveLeft();
	void OnAttack();

	void UpdateMovement() const;
};
