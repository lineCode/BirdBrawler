#pragma once

#include "CoreMinimal.h"

class ABirdBrawlerCharacter;

class BIRDBRAWLER_API FCombatUtils
{
public:
	FCombatUtils();
	~FCombatUtils();

	static void ApplyKnockbackTo(const FVector& Direction, float Force, ABirdBrawlerCharacter* TargetActor);
};
