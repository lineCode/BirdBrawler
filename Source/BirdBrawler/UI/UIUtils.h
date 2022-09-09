#pragma once

#include "CoreMinimal.h"

class AUIViewsHandler;

class BIRDBRAWLER_API UIUtils
{
public:
	static AUIViewsHandler* GetViewsHandler(const UWorld* World);
};
