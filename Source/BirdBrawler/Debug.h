#pragma once

#include "CoreMinimal.h"

#define BB_SLOG(EXPR) Debug::ScreenLog(EXPR, 2.f, FColor::White);
#define BB_SLOGK(KEY,EXPR) Debug::ScreenLog(KEY, EXPR, 2.f, FColor::White);

#define BB_SLOG_WARN(EXPR) Debug::ScreenLog(EXPR, 2.f, FColor::Yellow);
#define BB_SLOGK_WARN(KEY,EXPR) Debug::ScreenLog(KEY, EXPR, 2.f, FColor::Yellow);

#define BB_SLOG_ERR(EXPR) Debug::ScreenLog(EXPR, 2.f, FColor::Red);
#define BB_SLOGK_ERR(KEY,EXPR) Debug::ScreenLog(KEY, EXPR, 2.f, FColor::Red);

class BIRDBRAWLER_API Debug
{
public:
	Debug();
	~Debug();

	static void ScreenLog(const FString& Message, float Duration = 2.f, FColor Color = FColor::Yellow);
	static void ScreenLog(int Key, const FString& Message, float Duration = 2.f, FColor Color = FColor::Yellow);
};
