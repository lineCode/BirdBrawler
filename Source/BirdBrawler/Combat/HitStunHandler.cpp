// Copyright (c) Giammarco Agazzotti


#include "HitStunHandler.h"

#include "Kismet/GameplayStatics.h"


AHitStunHandler::AHitStunHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHitStunHandler::BeginPlay()
{
	Super::BeginPlay();
}

void AHitStunHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (FHitStunData& HitStunData : TargetsTimeTracker)
	{
		if (HitStunData.RemainingTime > 0.f)
		{
			if (!HitStunData.Dirty)
			{
				if (HitStunData.Shake)
				{
					UpdateTargetShake(HitStunData.TargetCharacter);
				}

				HitStunData.RemainingTime -= DeltaTime;
			}
		}
		else
		{
			HitStunData.TargetCharacter->DisableHitStun();
			StopTargetShake(HitStunData);

			HitStunData.Dirty = true;
		}
	}

	for (int i = TargetsTimeTracker.Num() - 1; i >= 0; --i)
	{
		if (TargetsTimeTracker[i].Dirty)
		{
			TargetsTimeTracker.RemoveAt(i);
		}
	}
}

void AHitStunHandler::ApplyHitStun(ABirdBrawlerCharacter* Target, float Duration, bool Shake /*= false*/)
{
	verify(Target);

	uint32 TargetId = Target->GetUniqueID();
	if (!GetHitStunData(TargetId))
	{
		TargetsTimeTracker.Emplace(FHitStunData(Target, Duration, Shake));
	}

	FHitStunData* Data = GetHitStunData(TargetId);
	verify(Data);

	Target->EnableHitStun(Shake);
}

FHitStunData* AHitStunHandler::GetHitStunData(uint32 TargetId)
{
	return TargetsTimeTracker.FindByPredicate([&](const FHitStunData& CurrentData) { return CurrentData.TargetCharacter->GetUniqueID() == TargetId; });
}

void AHitStunHandler::UpdateTargetShake(ABirdBrawlerCharacter* Target)
{
	verify(Target);

	float ElapsedTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	float DeltaY = FMath::Sin(ElapsedTime * MeshShakeFrequency) * MeshShakeAmplitude;

	auto* Mesh = Target->GetSkeletalMeshComponent();
	verify(Mesh);

	FVector CurrentPosition = Mesh->GetRelativeLocation();
	CurrentPosition.X += DeltaY;

	Mesh->SetRelativeLocation(CurrentPosition, false, nullptr, ETeleportType::TeleportPhysics);
}

void AHitStunHandler::StopTargetShake(const FHitStunData& HitStunData)
{
	verify(HitStunData.TargetCharacter);

	auto* Mesh = HitStunData.TargetCharacter->GetSkeletalMeshComponent();
	verify(Mesh);

	Mesh->SetRelativeLocation(HitStunData.OriginalMeshRelativeLocation);
}
