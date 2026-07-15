// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WeaponComponent.h"
#include "Player/HFPlayer.h"
#include "Player/ThrownWeapon.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultWeapon.WeaponType = EWeaponType::Pistol;
	DefaultWeapon.MaxAmmoCount = 16;
	DefaultWeapon.CurrentAmmoCount = 16;
	DefaultWeapon.FireRate = 0.15f;
	DefaultWeapon.bIsAutomatic = false;
	DefaultWeapon.BaseDamage = 10.0f;
	DefaultWeapon.StunDamage = 20.0f;
	DefaultWeapon.MaxRange = 5000.0f;
	DefaultWeapon.WeaponWorldMesh = nullptr;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(DefaultWeapon);
}

void UWeaponComponent::StartFiring()
{
	if(CurrentWeapon.CurrentAmmoCount == 0)
	{
		Reload();
		return;
	}

	if(CurrentWeapon.bIsAutomatic)
	{
		FireExecution();
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UWeaponComponent::FireExecution, CurrentWeapon.FireRate, true);
	}
	else
	{
		FireExecution();
	}
}

void UWeaponComponent::StopFiring()
{
	if(CurrentWeapon.bIsAutomatic)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		bCanFire = true;
	}
}

void UWeaponComponent::FireExecution()
{
	if(CurrentWeapon.CurrentAmmoCount <= 0)
	{
		StopFiring();
		if(CurrentWeapon.WeaponType != EWeaponType::Pistol)
		{
			EquipWeapon(DefaultWeapon);
		}

		return;
	}

	if(!CurrentWeapon.bIsAutomatic && !bCanFire)
	{
		return;
	}

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if(!PawnOwner)
	{
		return;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	PawnOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector LookDirection = EyeRotation.Vector();

	CurrentWeapon.CurrentAmmoCount--;
	OnAmmoCountChanged.Broadcast(CurrentWeapon.CurrentAmmoCount, CurrentWeapon.MaxAmmoCount);

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
			FVector SpreadDirection = (EyeLocation + RandomSpread).GetSafeNormal();
			FVector TraceEnd = LookDirection + (SpreadDirection * CurrentWeapon.MaxRange);

			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
			if(bHit)
			{
				if(AActor* HitActor = HitResult.GetActor())
				{
					FDamageEvent DamageEvent;
					HitActor->TakeDamage(CurrentWeapon.BaseDamage, DamageEvent, nullptr, GetOwner());
				}
			}
#if WITH_EDITOR
			FVector LineEnd = bHit ? HitResult.ImpactPoint : TraceEnd;
			FColor LineColor = bHit ? FColor::Red : FColor::Green;
			DrawDebugLine(GetWorld(), EyeLocation, LineEnd, LineColor, false, 1.0f, 0, 1.5f);
#endif
		}
	}
	// Profile Branch 2: Pistol, Deagle, SMG
	else
	{
		FHitResult HitResult;
		FVector RandomForward = FMath::VRandCone(LookDirection, FMath::DegreesToRadians(SpreadDegrees));
		FVector TraceEnd = EyeLocation + (RandomForward * CurrentWeapon.MaxRange);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
		if(bHit)
		{
			if(AActor* HitActor = HitResult.GetActor())
			{
				FDamageEvent DamageEvent;
				HitActor->TakeDamage(CurrentWeapon.BaseDamage, DamageEvent, nullptr, GetOwner());
			}
		}
#if WITH_EDITOR
		FVector LineEnd = bHit ? HitResult.ImpactPoint : TraceEnd;
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), EyeLocation, LineEnd, LineColor, false, 1.0f, 0, 1.5f);
#endif

		// TODO: Target Posture tracking(head->stun damage)
	}

	if(!CurrentWeapon.bIsAutomatic)
	{
		bCanFire = false;
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UWeaponComponent::ResetFireCooldown, CurrentWeapon.FireRate, false);
	}
}

void UWeaponComponent::Reload()
{
	if(CurrentWeapon.WeaponType == EWeaponType::Pistol)
	{
		CurrentWeapon.CurrentAmmoCount = CurrentWeapon.MaxAmmoCount;
		OnAmmoCountChanged.Broadcast(CurrentWeapon.CurrentAmmoCount, CurrentWeapon.MaxAmmoCount);
	}
}

void UWeaponComponent::EquipWeapon(const FWeaponData& NewWeaponData)
{
	CurrentWeapon = NewWeaponData;
	OnAmmoCountChanged.Broadcast(CurrentWeapon.CurrentAmmoCount, CurrentWeapon.MaxAmmoCount);
	// TODO: Equipping animation
	bCanFire = true;

	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
}

void UWeaponComponent::ThrowWeapon()
{
	if(CurrentWeapon.WeaponType == EWeaponType::Pistol)
	{
		return;
	}
	if(!ThrownWeaponClass)
	{
		return;
	}

	// Safety shutoff
	StopFiring();

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if(!PawnOwner)
	{
		return;
	}
	FVector LaunchLocation;
	FRotator LaunchRotation;
	PawnOwner->GetActorEyesViewPoint(LaunchLocation, LaunchRotation);

	FVector LaunchDirection = LaunchRotation.Vector();

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

	EquipWeapon(DefaultWeapon);
}

void UWeaponComponent::ResetFireCooldown()
{
	bCanFire = true;
}

