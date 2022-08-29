#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovesEffectorComponent.generated.h"

class ABirdBrawlerCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BIRDBRAWLER_API UMovesEffectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovesEffectorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// TODO: this will not only hit ABirdBrawlerCharacter but also other entities (make hittable entity?)
	void ExecuteMove(const FVector& Knockback, const ABirdBrawlerCharacter& Target);
};
