﻿#include "CombatUtils.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"

void FCombatUtils::ApplyKnockbackTo(const FVector& Direction, float Force, ABirdBrawlerCharacter* TargetActor)
{
	verify(TargetActor);

	float MultipliedForce = Force * TargetActor->GetKnockbackMultiplier();
	TargetActor->LaunchCharacter(Direction.GetSafeNormal() * MultipliedForce, true, true);
}

void FCombatUtils::FaceTargetCharacter(ABirdBrawlerCharacter* Character, ABirdBrawlerCharacter* Target)
{
	verify(Character);
	verify(Target);

	bool TargetIsOnTheRightSide = Target->GetActorLocation().Y > Character->GetActorLocation().Y;

	Target->SetActorRotation(FRotator(0, TargetIsOnTheRightSide ? -90 : 90, 0));
}
