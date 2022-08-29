#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MoveEndedNotify.generated.h"

UCLASS()
class BIRDBRAWLER_API UMoveEndedNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	FName MoveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	bool ResetMoveAfterNotify{true};

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
