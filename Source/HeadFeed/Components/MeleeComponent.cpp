// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MeleeComponent.h"
#include "Player/HFPlayer.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

UMeleeComponent::UMeleeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMeleeComponent::Attack()
{
	if(!bCanMelee)
	{
		return;
	}

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if(!PawnOwner) return;

	FVector EyeLocation;
	FRotator EyeRotation;
	PawnOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector LookDirection = EyeRotation.Vector();
	FVector TraceEnd = EyeLocation + (LookDirection * MaxRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FCollisionShape MeleeShape = FCollisionShape::MakeCapsule(SwingWidth, SwingHeight);
	FQuat MeleeRotation = EyeRotation.Quaternion() * SwingRotation.Quaternion();

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, EyeLocation, TraceEnd, MeleeRotation, ECC_Visibility, MeleeShape, QueryParams);
	if(bHit)
	{
		TArray<AActor*> HitActors;
		for(const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if(HitActor && !HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);

				FDamageEvent DamageEvent;
				HitActor->TakeDamage(MeleeDamage, DamageEvent, PawnOwner->GetController(), GetOwner());

				// TODO: Insert your execution/parasite health-feeding logic here.
				// e.g., if (HitActor->IsStunned()) { GivePlayerHealth(LifestealAmount); }
			}
		}
	}
#if WITH_EDITOR
	FColor DebugColor = bHit ? FColor::Magenta : FColor::Cyan;

	DrawDebugCapsule(GetWorld(), TraceEnd, SwingHeight, SwingWidth, MeleeRotation, DebugColor, false, 0.4f, 0, 1.5f);
#endif

	bCanMelee = false;
	GetWorld()->GetTimerManager().SetTimer(MaleeRateTimerHandle, this, &UMeleeComponent::ResetCooldown, MeleeRate, false);
}

void UMeleeComponent::ResetCooldown()
{
	bCanMelee = true;
}
