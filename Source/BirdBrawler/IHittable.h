#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHittable.generated.h"

UINTERFACE()
class UHittable : public UInterface
{
	GENERATED_BODY()
};

class BIRDBRAWLER_API IHittable
{
	GENERATED_BODY()

public:
	virtual void OnHit(const FVector& Knockback, const ACharacter& Hitter);
};
