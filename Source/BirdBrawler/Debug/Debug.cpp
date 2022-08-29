#include "Debug.h"
#include "Engine/GameEngine.h"

Debug::Debug()
{
}

Debug::~Debug()
{
}

void Debug::ScreenLog(const FString& Message, float Duration, FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
	}
}

void Debug::ScreenLog(const int Key, const FString& Message, float Duration, FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Message);
	}
}
