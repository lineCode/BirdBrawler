#pragma once

#include "CoreMinimal.h"
#include "UIViewsHandler.h"

class BIRDBRAWLER_API UIUtils
{
public:
	static AUIViewsHandler* GetViewsHandler(const UWorld* World);
};
