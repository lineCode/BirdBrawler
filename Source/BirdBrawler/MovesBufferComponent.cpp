#include "MovesBufferComponent.h"

#include "Debug.h"

namespace
{
	const FString NoInput{""};

	const FString EntryStartJump{"StartJump"};
	const FString EntryStopJump{"StopJump"};
	const FString EntryStartMoveRight{"StartMoveRight"};
	const FString EntryStartMoveLeft{"StartMoveLeft"};
	const FString EntryStopMove{"StopMove"};
	const FString EntryAttack{"Attack"};
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

	if (auto* Input = GetOwner()->InputComponent)
	{
		Input->BindAction("Jump", IE_Pressed, this, &UMovesBufferComponent::OnStartJump);
		Input->BindAction("Jump", IE_Released, this, &UMovesBufferComponent::OnStopJump);

		Input->BindAction("MoveRight", IE_Pressed, this, &UMovesBufferComponent::OnStartMoveRight);
		Input->BindAction("MoveRight", IE_Released, this, &UMovesBufferComponent::OnStopMoveRight);

		Input->BindAction("MoveLeft", IE_Pressed, this, &UMovesBufferComponent::OnStartMoveLeft);
		Input->BindAction("MoveLeft", IE_Released, this, &UMovesBufferComponent::OnStopMoveLeft);

		Input->BindAction("Attack", IE_Pressed, this, &UMovesBufferComponent::OnAttack);
	}

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

	MovingRight = true;
}

void UMovesBufferComponent::OnStopMoveRight()
{
	AddMoveToBuffer(EntryStopMove);

	MovingRight = false;
}

void UMovesBufferComponent::OnStartMoveLeft()
{
	AddMoveToBuffer(EntryStartMoveLeft);

	MovingLeft = true;
}

void UMovesBufferComponent::OnStopMoveLeft()
{
	AddMoveToBuffer(EntryStopMove);

	MovingLeft = false;
}

void UMovesBufferComponent::OnAttack()
{
	AddMoveToBuffer(EntryAttack);
}

void UMovesBufferComponent::UpdateMovement() const
{
	if (auto* Character = Cast<ABirdBrawlerCharacter>(GetOwner()))
	{
		if ((MovingRight && MovingLeft) || (!MovingRight && !MovingLeft))
		{
			Character->SetMovementDirection(0.f);
		}
		else
		{
			if (MovingRight)
			{
				Character->SetMovementDirection(1.f);
			}
			else if (MovingLeft)
			{
				Character->SetMovementDirection(-1.f);
			}
		}
	}
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

		BB_SLOGK(i, Message);
	}

	BufferChanged = false;

	UpdateMovement();
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
