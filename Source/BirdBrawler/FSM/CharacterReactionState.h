// Copyright (c) Giammarco Agazzotti

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateBase.h"
#include "UObject/Object.h"
#include "CharacterReactionState.generated.h"

UCLASS()
class BIRDBRAWLER_API UCharacterReactionState : public UCharacterStateBase
{
	GENERATED_BODY()

protected:
	virtual void Enter_Implementation() override;
	virtual void Exit_Implementation() override;
};
