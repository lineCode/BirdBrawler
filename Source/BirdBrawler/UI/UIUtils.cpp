#include "UIUtils.h"

#include "Kismet/GameplayStatics.h"

AUIViewsHandler* UIUtils::GetViewsHandler(const UWorld* World)
{
	AActor* ViewsHandlerActor = UGameplayStatics::GetActorOfClass(World, AUIViewsHandler::StaticClass());
	verify(ViewsHandlerActor);

	AUIViewsHandler* ViewsHandler = Cast<AUIViewsHandler>(ViewsHandlerActor);
	return ViewsHandler;
}
