#pragma once

#include "CoreMinimal.h"
#include "HitStunHandler.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"

class ABirdBrawlerCharacter;

#define MIN_HITSTUN_TIME_DILATION 0.005f

class BIRDBRAWLER_API FCombatUtils
{
public:
	static AHitStunHandler* GetHitStunHandler(UWorld* World);

	static float ApplyKnockbackTo(const FVector& Direction, float Force, ABirdBrawlerCharacter* TargetActor, bool IgnoreKnockbackMultiplier = false);
	static void FaceTargetCharacter(ABirdBrawlerCharacter* Character, ABirdBrawlerCharacter* Target);
	static void ApplyHitStunTo(ABirdBrawlerCharacter* TargetCharacter, float HitStunDuration, bool Shake);
};
