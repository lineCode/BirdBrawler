#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include <deque>

#include "BirdBrawlerCharacter.h"
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

	float InputMovement{0.f};

	bool MovingRight{false};
	bool MovingLeft{false};

protected:
	std::deque<FInputBufferEntry> Buffer;
	float ElapsedFrameTime = 0.f;
	bool BufferChanged{false};

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AddMoveToBuffer(const FString& MoveName);
	bool BufferContainsConsumableInput(const FString& Input) const;

	void ClearBuffer();

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
