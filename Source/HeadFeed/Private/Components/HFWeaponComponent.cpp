// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HFWeaponComponent.h"
#include "Components/ThrownWeapon.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"


UHFWeaponComponent::UHFWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultWeaponPistol.WeaponType = EWeaponType::Pistol;
	DefaultWeaponPistol.MaxAmmoCount = 16;
	DefaultWeaponPistol.CurrentAmmoCount = 16;
	DefaultWeaponPistol.FireRate = 0.15f;
	DefaultWeaponPistol.BaseDamage = 10.0f;
	DefaultWeaponPistol.StunDamage = 20.0f;
	DefaultWeaponPistol.MaxRange = 5000.0f;
	DefaultWeaponPistol.WeaponWorldMesh = nullptr;
}

void UHFWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(DefaultWeaponPistol);
}

void UHFWeaponComponent::StartFiring(const FVector& EyeLocation, const FVector& LookDirection)
{
	CachedEyeLocation = EyeLocation;
	CachedLookDirection = LookDirection;

	if(CurrentWeapon.bIsAutomatic)
	{
		FireExecutionLoop();
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UHFWeaponComponent::FireExecutionLoop, CurrentWeapon.FireRate, true);
	}
	else
	{
		FireExecutionLoop();
	}
}

void UHFWeaponComponent::StopFiring()
{
	if(CurrentWeapon.bIsAutomatic)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		bCanFire = true;
	}
}

void UHFWeaponComponent::FireExecutionLoop()
{
	if(CurrentWeapon.CurrentAmmoCount <= 0)
	{
		StopFiring();
		if(CurrentWeapon.WeaponType != EWeaponType::Pistol)
		{
			EquipWeapon(DefaultWeaponPistol);
		}

		return;
	}

	if(!CurrentWeapon.bIsAutomatic && !bCanFire)
	{
		return;
	}

	CurrentWeapon.CurrentAmmoCount--;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	// Profile Branch 1: Shotgun multi-trace calc
	if(CurrentWeapon.WeaponType == EWeaponType::Shotgun)
	{
		const int32 PelletCount = 8;
		for(int32 i = 0; i < PelletCount; i++)
		{
			FHitResult HitResult;

			FVector RandomSpread = FVector::UpVector * FMath::FRandRange(-0.06f, 0.06f) + FVector::RightVector * FMath::FRandRange(-0.06f, 0.06f);
			FVector SpreadDirection = (CachedLookDirection + RandomSpread).GetSafeNormal();
			FVector TraceEnd = CachedEyeLocation + (SpreadDirection * CurrentWeapon.MaxRange);

			if(GetWorld()->LineTraceSingleByChannel(HitResult, CachedEyeLocation, TraceEnd, ECC_Visibility, QueryParams))
			{
				if(AActor* HitActor = HitResult.GetActor())
				{
					FDamageEvent DamageEvent;
					HitActor->TakeDamage(CurrentWeapon.BaseDamage, DamageEvent, nullptr, GetOwner());
				}
			}
		}
	}
	// Profile Branch 2: Pistol, Deagle, SMG
	else
	{
		FHitResult HitResult;

		FVector TraceEnd = CachedEyeLocation + (CachedLookDirection * CurrentWeapon.MaxRange);

		if(GetWorld()->LineTraceSingleByChannel(HitResult, CachedEyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			if(AActor* HitActor = HitResult.GetActor())
			{
				FDamageEvent DamageEvent;
				HitActor->TakeDamage(CurrentWeapon.BaseDamage, DamageEvent, nullptr, GetOwner());
			}
		}

		// TODO: Target Posture tracking(head->stun damage)
	}

	if(!CurrentWeapon.bIsAutomatic)
	{
		bCanFire = false;
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UHFWeaponComponent::ResetFireCooldown, CurrentWeapon.FireRate, false);
	}
}

void UHFWeaponComponent::Reload()
{
	if(CurrentWeapon.WeaponType == EWeaponType::Pistol)
	{
		CurrentWeapon.CurrentAmmoCount = CurrentWeapon.MaxAmmoCount;
	}
}

void UHFWeaponComponent::EquipWeapon(const FWeaponData& NewWeaponData)
{
	CurrentWeapon = NewWeaponData;
	// TODO: Equipping animation
	bCanFire = true;

	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
}

void UHFWeaponComponent::ThrowWeapon(const FVector& LaunchLocation, const FVector& LaunchDirection)
{
	if(CurrentWeapon.WeaponType == EWeaponType::Pistol)
	{
		return;
	}
	
	// Safety shutoff
	StopFiring();

	if(ThrownWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FRotator SpawnRotation = LaunchDirection.Rotation();
		AThrownWeapon* FlyingGun = GetWorld()->SpawnActor<AThrownWeapon>(ThrownWeaponClass, LaunchLocation, SpawnRotation, SpawnParams);
		if(FlyingGun)
		{
			FlyingGun->InitializeThrow(CurrentWeapon.WeaponWorldMesh, CurrentWeapon.StunDamage, CurrentWeapon.BaseDamage);
		}
	}

	EquipWeapon(DefaultWeaponPistol);
}

void UHFWeaponComponent::ResetFireCooldown()
{
	bCanFire = true;
}

