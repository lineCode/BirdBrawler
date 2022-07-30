#include "HitboxNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"

void UHitboxNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	const auto& SocketLocation = MeshComp->GetSocketLocation(SocketName);

	TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceTypes;
	const auto PawnCollisionType = UEngineTypes::ConvertToObjectType(ECC_Pawn);
	TargetTraceTypes.Add(PawnCollisionType);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MeshComp->GetOwner());

	FHitResult OutHit;
	const auto* World = MeshComp->GetWorld();
	UKismetSystemLibrary::SphereTraceSingleForObjects(World, SocketLocation, SocketLocation, 30.f,
	                                                  TargetTraceTypes, false, ActorsToIgnore,
	                                                  EDrawDebugTrace::ForDuration, OutHit, true);
}
