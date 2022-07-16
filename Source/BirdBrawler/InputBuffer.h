#pragma once

#include <deque>

#include "CoreMinimal.h"
#include "InputBuffer.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Input))
class BIRDBRAWLER_API UInputBuffer : public UActorComponent
{
	GENERATED_BODY()

	struct FInputBufferEntry
	{
		FString Name;
		bool Used;
	};

public:
	UInputBuffer() = default;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool IsInputBuffered(const FString& Input, bool ConsumeEntry = true);

private:
	std::deque<FInputBufferEntry> Buffer;

	float ElapsedFrameTime = 0.f;
	bool BufferChanged{false};

	void ClearBuffer();

	void OnJumpAction();

	UInputComponent* InputComponent{nullptr};
};
