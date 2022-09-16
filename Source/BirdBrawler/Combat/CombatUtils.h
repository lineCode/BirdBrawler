#pragma once

#include "CoreMinimal.h"
#include "HitStunHandler.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"

class ABirdBrawlerCharacter;

class BIRDBRAWLER_API FCombatUtils
{
public:
	static AHitStunHandler* GetHitStunHandler(UWorld* World);

	static void ApplyKnockbackTo(const FVector& Direction, float Force, ABirdBrawlerCharacter* TargetActor, bool IgnoreKnockbackMultiplier = false);
	static void FaceTargetCharacter(ABirdBrawlerCharacter* Character, ABirdBrawlerCharacter* Target);
	static void ApplyHitStunTo(ABirdBrawlerCharacter* TargetCharacter, float HitStunDuration, bool Shake);
};
