#include "MovesBufferComponent.h"

#include "Debug.h"

namespace
{
	constexpr auto BufferSizeFrames{6};
	constexpr auto BufferFrameLength{.032f};
	const FString NoInput{""};

	const FString EntryStartJump{"StartJump"};
	const FString EntryStopJump{"StopJump"};
	const FString EntryStartMoveRight{"StartMoveRight"};
	const FString EntryStartMoveLeft{"StartMoveLeft"};
	const FString EntryStopMove{"StopMove"};
}

UMovesBufferComponent::UMovesBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UMovesBufferComponent::IsInputBuffered(const FString& Input, bool ConsumeEntry)
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

TArray<FString> UMovesBufferComponent::GetBufferedInputs() const
{
	TArray<FString> BufferedInputs;
	for (auto& Input : Buffer)
	{
		if (Input.Name != NoInput && !Input.Used)
		{
			BufferedInputs.Emplace(Input.Name);
		}
	}

	return BufferedInputs;
}

void UMovesBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->InputComponent->BindAction("Jump", IE_Pressed, this, &UMovesBufferComponent::OnStartJump);
	GetOwner()->InputComponent->BindAction("Jump", IE_Released, this, &UMovesBufferComponent::OnStopJump);

	GetOwner()->InputComponent->BindAction("MoveRight", IE_Pressed, this,
	                                       &UMovesBufferComponent::OnStartMoveRight);
	GetOwner()->InputComponent->BindAction("MoveRight", IE_Released, this,
	                                       &UMovesBufferComponent::OnStopMoveRight);
	GetOwner()->InputComponent->BindAction("MoveLeft", IE_Pressed, this,
	                                       &UMovesBufferComponent::OnStartMoveLeft);
	GetOwner()->InputComponent->BindAction("MoveLeft", IE_Released, this,
	                                       &UMovesBufferComponent::OnStopMoveLeft);

	GetOwner()->InputComponent->BindAxis("MoveHorizontal", this, &UMovesBufferComponent::OnMoveHorizontal);

	ClearBuffer();
}

void UMovesBufferComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMovesBufferComponent::AddMoveToBuffer(const FString& MoveName)
{
	Buffer.emplace_back(FInputBufferEntry{FString(MoveName), false});
	Buffer.pop_front();

	BufferChanged = true;
}

bool UMovesBufferComponent::BufferContainsConsumableInput(const FString& Input) const
{
	for (const auto& CurrentInput : Buffer)
	{
		if (CurrentInput.Name == Input && !CurrentInput.Used)
		{
			return true;
		}
	}

	return false;
}

void UMovesBufferComponent::ClearBuffer()
{
	Buffer.clear();
	for (int i = 0; i < BufferSizeFrames; ++i)
	{
		Buffer.emplace_back(FInputBufferEntry{NoInput, false});
	}
}

void UMovesBufferComponent::OnMoveHorizontal(float Value)
{
	InputMovement = Value;
}

// TODO: these are all the same, make it generic maybe?
void UMovesBufferComponent::OnStartJump()
{
	AddMoveToBuffer(EntryStartJump);
}

void UMovesBufferComponent::OnStopJump()
{
	AddMoveToBuffer(EntryStopJump);
}

void UMovesBufferComponent::OnStartMoveRight()
{
	AddMoveToBuffer(EntryStartMoveRight);
}

void UMovesBufferComponent::OnStopMoveRight()
{
	AddMoveToBuffer(EntryStopMove);
}

void UMovesBufferComponent::OnStartMoveLeft()
{
	AddMoveToBuffer(EntryStartMoveLeft);
}

void UMovesBufferComponent::OnStopMoveLeft()
{
	AddMoveToBuffer(EntryStopMove);
}

void UMovesBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedFrameTime += DeltaTime;

	if (ElapsedFrameTime >= BufferFrameLength)
	{
		ElapsedFrameTime = 0.f;

		if (!BufferChanged)
		{
			AddMoveToBuffer(NoInput);
		}
	}

	for (int i = 0; i < Buffer.size(); ++i)
	{
		auto& Entry = Buffer.at(i);
		const bool IsEmpty = Entry.Name == NoInput;
		auto Message = IsEmpty ? "Empty" : Entry.Name;
		const auto Color = IsEmpty ? FColor::White : FColor::Green;

		Debug::ScreenLog(i, Message, 2.f, Color);
	}

	BufferChanged = false;
}

void UMovesBufferComponent::UseBufferedInput(const FString& Input)
{
	verify(BufferContainsConsumableInput(Input));

	for (auto& CurrentInput : Buffer)
	{
		if (CurrentInput.Name == Input)
		{
			CurrentInput.Used = true;
		}
	}
}
