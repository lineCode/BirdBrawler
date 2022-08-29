#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultipleTargetsCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BIRDBRAWLER_API UMultipleTargetsCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMultipleTargetsCameraComponent();

protected:
	UPROPERTY(EditAnywhere)
	float MinDistance{0.f};

	UPROPERTY(EditAnywhere)
	bool AutoAddTargetsOnBeginPlay{true};

	UPROPERTY(EditAnywhere)
	float MovementDamping{2.f};

	UPROPERTY(EditAnywhere)
	FVector PositionOffset;

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool AddTarget(AActor* Target);
	bool RemoveTarget(AActor* Target);

private:
	UPROPERTY()
	TArray<AActor*> Targets;

	UPROPERTY()
	TMap<AActor*, FVector> TargetsCentersMap;

	void AddAvailableTargets();

	FVector GetCenterPosition() const;
	void UpdateCameraPosition(float DeltaTime) const;
};
