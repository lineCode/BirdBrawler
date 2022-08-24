#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class BIRDBRAWLER_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void CurrentMoveChanged(FName CurrentMoveName);
};
