#pragma once

#include "CoreMinimal.h"
#include "BirdBrawler/Characters/BirdBrawlerCharacter.h"
#include "PushCharacterNotify.generated.h"

UCLASS()
class BIRDBRAWLER_API UPushCharacterNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	EPushDirection PushDirection;

	UPROPERTY(EditAnywhere)
	float Intensity = 1.f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
