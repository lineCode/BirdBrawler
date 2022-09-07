#include "GameFlowStateBase.h"

#include "BirdBrawler/UI/UISubsystem.h"
#include "Kismet/GameplayStatics.h"

void UGameFlowStateBase::ShowView(const FString& Id)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	verify(GameInstance);

	UUISubsystem* UI = GameInstance->GetSubsystem<UUISubsystem>();
	verify(UI);

	UI->ShowView(Id);
}
