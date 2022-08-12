#include "HitboxNotifyState.h"

#include "BirdBrawlerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

void UHitboxNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	HitPawsIds.Empty();
}

void UHitboxNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	HitPawsIds.Empty();
}

void UHitboxNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	const FVector& SocketLocation = MeshComp->GetSocketLocation(SocketName);

	if (HitboxDataAsset)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceTypes;

		// TODO: filter by "hittable character" not by pawn
		const EObjectTypeQuery PawnCollisionType = UEngineTypes::ConvertToObjectType(ECC_Pawn);
		TargetTraceTypes.Add(PawnCollisionType);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(MeshComp->GetOwner());

		FHitResult OutHit;
		const UWorld* World = MeshComp->GetWorld();
		const bool DidHit = UKismetSystemLibrary::SphereTraceSingleForObjects(World, SocketLocation, SocketLocation,
		                                                                      HitboxDataAsset->Radius,
		                                                                      TargetTraceTypes, false, ActorsToIgnore,
		                                                                      EDrawDebugTrace::None, OutHit, true);


		DrawDebugSphere(MeshComp->GetWorld(), SocketLocation, HitboxDataAsset->Radius, 7, FColor::Red, false);

		const FRotator Rotator = FRotator(HitboxDataAsset->KnockbackOrientation, 90.f, 0.f);
		const FVector KnockbackVector = Rotator.RotateVector(FVector(1.f, 0.f, 0.f));

		const FVector EndPoint = SocketLocation + KnockbackVector * 50.f;

		if (DidHit && !HitPawsIds.Contains(OutHit.Actor->GetUniqueID()))
		{
			HitPawsIds.Emplace(OutHit.Actor->GetUniqueID());

			if (auto* Hittable = Cast<IHittable>(OutHit.Actor))
			{
				Hittable->OnHit(KnockbackVector, *MeshComp->GetOwner());
			}
		}

		DrawDebugDirectionalArrow(MeshComp->GetWorld(), SocketLocation, EndPoint, 10.f, FColor::Green, false);
	}
}
