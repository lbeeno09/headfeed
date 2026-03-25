// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HFCombatComponent.h"
#include "Components/HFLifeLinkComponent.h"
#include "Interfaces/HFCombatInterface.h"
#include "Kismet/GameplayStatics.h"

UHFCombatComponent::UHFCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHFCombatComponent::PrimaryAttack()
{
	ExecuteHitscan();
}

void UHFCombatComponent::SecondaryAttack()
{
	ExecuteMeleeAction();
}

void UHFCombatComponent::ExecuteHitscan()
{
	AActor* Owner = GetOwner();
	if(!Owner)
	{
		return;
	}

	FVector CameraLoc;
	FRotator CameraRot;
	Owner->GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector End = CameraLoc + (CameraRot.Vector() * MaxRange);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	if(GetWorld()->LineTraceSingleByChannel(Hit, CameraLoc, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if(!HitActor)
		{
			return;
		}

		float DamageToApply = (Hit.BoneName == HeadBoneName) ? (GunDamage * HeadShotMultiplier) : GunDamage;
		UGameplayStatics::ApplyDamage(HitActor, DamageToApply, Owner->GetInstigatorController(), Owner, nullptr);

	}
}

void UHFCombatComponent::ExecuteMeleeAction()
{
	AActor* Owner = GetOwner();
	if(!Owner)
	{
		return;
	}

	FVector CameraLoc;
	FRotator CameraRot;
	Owner->GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector End = CameraLoc + (CameraRot.Vector() * MeleeDistance);

	FHitResult Hit;
	FCollisionShape MeleeShape = FCollisionShape::MakeSphere(50.0f);

	if(GetWorld()->SweepSingleByChannel(Hit, CameraLoc, End, FQuat::Identity, ECC_Visibility, MeleeShape))
	{
		AActor* HitActor = Hit.GetActor();
		if(!HitActor)
		{
			return;
		}

		if(IHFCombatInterface* Target = Cast<IHFCombatInterface>(HitActor))
		{
			if(Target->IsStunned())
			{
				Target->ExecuteFinisher();

				if(UHFLifeLinkComponent* LifeLink = Owner->FindComponentByClass<UHFLifeLinkComponent>())
				{
					LifeLink->ModifyHealth(FinisherHealAmount, true);
				}

				return;
			}
		}

		UGameplayStatics::ApplyDamage(HitActor, GunDamage * 0.7f, Owner->GetInstigatorController(), Owner, nullptr);
	}
}
