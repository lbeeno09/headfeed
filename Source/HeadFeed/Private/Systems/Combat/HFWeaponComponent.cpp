// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/Combat/HFWeaponComponent.h"
#include "Interfaces/HFDamagable.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HFPlayer.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"

UHFWeaponComponent::UHFWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHFWeaponComponent::Fire()
{
	if(CurrentAmmo <= 0)
	{
		Reload();
		return;
	}
	CurrentAmmo--;
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);

	if(GEngine)
	{
		FString Message = FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, Message);
	}

	//PlayFireEffects();

	AActor* Owner = GetOwner();
	if(!Owner)
	{
		return;
	}

	FVector Start;
	FRotator Rotation;
	Owner->GetActorEyesViewPoint(Start, Rotation);
	FVector End = Start + (Rotation.Vector() * Range);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	if(GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 2.0f);

		AActor* HitActor = Hit.GetActor();
		if(HitActor && HitActor->Implements<UHFDamagable>())
		{
			IHFDamagable::Execute_ReceiveDamage(HitActor, Damage, Owner);

			float CurrentHP = IHFDamagable::Execute_GetHealth(HitActor);
			float MaxHP = IHFDamagable::Execute_GetMaxHealth(HitActor);

			FString Message = FString::Printf(TEXT("HIT: %s | HP: %.1f / %.1f"), *HitActor->GetName(), CurrentHP, MaxHP);

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, Message);
		}
	}
}

void UHFWeaponComponent::ThrowWeapon()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Weapon Thrown! Reverting State..."));
	}

	//PlayThrowingsEffects();

	AHFPlayer* Player = Cast<AHFPlayer>(GetOwner());
	if(!Player)
	{
		return;
	}

	FVector Start;
	FRotator Rotation;
	Player->GetActorEyesViewPoint(Start, Rotation);
	FVector LaunchVelocity = Rotation.Vector() * 2000.0f;

	// Spawn Weapon

	// Reset State
	Player->SetCombatState(EHFCombatState::SingleHandMelee);
}

void UHFWeaponComponent::Reload()
{
	if(CurrentAmmo >= MaxAmmo)
	{
		return;
	}

	CurrentAmmo = MaxAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Reloaded!"));
	}
}
