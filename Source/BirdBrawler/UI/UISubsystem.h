#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UISubsystem.generated.h"

UCLASS()
class BIRDBRAWLER_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void ShowView(const FString& Id);
};
