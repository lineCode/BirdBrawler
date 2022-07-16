#pragma once

#include "CoreMinimal.h"

class BIRDBRAWLER_API Debug
{
public:
	Debug();
	~Debug();

	static void ScreenLog(const FString& Message, float Duration = 2.f, FColor Color = FColor::Yellow);
	static void ScreenLog(int Key, const FString& Message, float Duration = 2.f, FColor Color = FColor::Yellow);
};
