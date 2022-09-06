#include "CombatUtils.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"


FCombatUtils::FCombatUtils()
{
}

FCombatUtils::~FCombatUtils()
{
}

void FCombatUtils::ApplyKnockbackTo(const FVector& Direction, float Force, ABirdBrawlerCharacter* TargetActor)
{
	verify(TargetActor);
	TargetActor->LaunchCharacter(Direction.GetSafeNormal() * Force, true, true);
}
