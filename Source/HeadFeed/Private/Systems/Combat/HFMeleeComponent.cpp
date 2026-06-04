// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/Combat/HFMeleeComponent.h"
#include "Interfaces/HFDamagable.h"

UHFMeleeComponent::UHFMeleeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHFMeleeComponent::Melee()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Melee Attack!!"));
	}

	//PlayMeleeEffects();

	AActor* Owner = GetOwner();
	if(!Owner)
	{
		return;
	}

	FVector Start = Owner->GetActorLocation();
	FVector Forward = Owner->GetActorForwardVector();
	FVector End = Start + (Forward * MeleeRange);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SweepRadius);
	if(GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor && HitActor->Implements<UHFDamagable>())
		{
			IHFDamagable::Execute_ReceiveDamage(HitActor, MeleeDamage, Owner);

			float CurrentHP = IHFDamagable::Execute_GetHealth(HitActor);
			float MaxHP = IHFDamagable::Execute_GetMaxHealth(HitActor);

			FString Message = FString::Printf(TEXT("HIT: %s | HP: %.1f / %.1f"), *HitActor->GetName(), CurrentHP, MaxHP);

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, Message);
		}
	}
}
