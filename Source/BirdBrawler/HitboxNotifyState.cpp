#include "HitboxNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

void UHitboxNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	const auto& SocketLocation = MeshComp->GetSocketLocation(SocketName);

	if (HitboxDataAsset)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceTypes;
		const auto PawnCollisionType = UEngineTypes::ConvertToObjectType(ECC_Pawn);
		TargetTraceTypes.Add(PawnCollisionType);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(MeshComp->GetOwner());

		FHitResult OutHit;
		const auto* World = MeshComp->GetWorld();
		UKismetSystemLibrary::SphereTraceSingleForObjects(World, SocketLocation, SocketLocation,
		                                                  HitboxDataAsset->Radius,
		                                                  TargetTraceTypes, false, ActorsToIgnore,
		                                                  EDrawDebugTrace::None, OutHit, true);

		DrawDebugSphere(MeshComp->GetWorld(), SocketLocation, HitboxDataAsset->Radius, 7, FColor::Red, false);

		const auto Rotator = FRotator(HitboxDataAsset->KnockbackOrientation, 90.f, 0.f);
		const auto KnockbackVector = Rotator.RotateVector(FVector(1.f, 0.f, 0.f));

		const auto EndPoint = SocketLocation + KnockbackVector * 50.f;

		DrawDebugDirectionalArrow(MeshComp->GetWorld(), SocketLocation, EndPoint, 10.f, FColor::Green, false);
	}
}
