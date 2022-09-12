#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class BIRDBRAWLER_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void CurrentMoveChanged(FName CurrentMoveName);

	UFUNCTION(BlueprintImplementableEvent)
	void AnimationRequested(UAnimSequenceBase* AnimationSequence, bool Loop);
};
