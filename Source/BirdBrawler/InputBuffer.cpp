#include "InputBuffer.h"

namespace
{
	constexpr auto BufferSizeFrames{6};
	constexpr auto BufferFrameLength{.32f};
	const FString NoInput{""};

	const FName ActionJump{"Jump"};
	const FString EntryJump{"Jump"};

	const FName ActionMoveRight{"MoveRight"};
	const FString EntryMoveRight{"MoveRight"};

	const FName ActionMoveLeft{"MoveLeft"};
	const FString EntryMoveLeft{"MoveLeft"};
}

void UInputBuffer::BeginPlay()
{
	UActorComponent::BeginPlay();

	InputComponent->BindAction(ActionJump, IE_Pressed, this, &UInputBuffer::OnJumpAction);
}

void UInputBuffer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UActorComponent::EndPlay(EndPlayReason);
}

void UInputBuffer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedFrameTime += DeltaTime;

	if (ElapsedFrameTime >= BufferFrameLength)
	{
		ElapsedFrameTime = 0.f;

		if (!BufferChanged)
		{
			Buffer.emplace_back(FInputBufferEntry{NoInput, false});
			Buffer.pop_front();
		}
	}

	BufferChanged = false;
}

bool UInputBuffer::IsInputBuffered(const FString& Input, const bool ConsumeEntry /*= true*/)
{
	for (int i = 0; i < Buffer.size(); ++i)
	{
		auto& Entry = Buffer.at(i);
		if (Entry.Name != NoInput && Entry.Name == Input && !Entry.Used)
		{
			if (ConsumeEntry)
			{
				Entry.Used = true;
			}

			return true;
		}
	}

	return false;
}

void UInputBuffer::ClearBuffer()
{
	Buffer.clear();
	for (int i = 0; i < BufferSizeFrames; ++i)
	{
		Buffer.emplace_back(FInputBufferEntry{NoInput, false});
	}
}

void UInputBuffer::OnJumpAction()
{
	Buffer.emplace_back(FInputBufferEntry{EntryJump, false});
	BufferChanged = true;
}
