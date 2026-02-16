// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HFWeapon.h"
#include "Weapons/HFWeaponProjectile.h"
#include "Weapons/HFWeaponHolder.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"

AHFWeapon::AHFWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(RootComponent);
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	FirstPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	FirstPersonMesh->bOnlyOwnerSee = true;

	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Third Person Mesh"));
	ThirdPersonMesh->SetupAttachment(RootComponent);
	ThirdPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	ThirdPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	ThirdPersonMesh->bOnlyOwnerSee = true;
}

void AHFWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// Sub to owners' destroyed delegate
	GetOwner()->OnDestroyed.AddDynamic(this, &AHFWeapon::OnOwnerDestroyed);

	WeaponOwner = Cast<IHFWeaponHolder>(GetOwner());
	PawnOwner = Cast<APawn>(GetOwner());

	CurrentBullets = MagazineSize;

	WeaponOwner->AttachWeaponMeshes(this);
}

void AHFWeapon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

void AHFWeapon::OnOwnerDestroyed(AActor* DestroyedActor)
{
	Destroy();
}

void AHFWeapon::ActivateWeapon()
{
	SetActorHiddenInGame(false);

	// Notify owner
	WeaponOwner->OnWeaponActivated(this);
}

void AHFWeapon::DeactivateWeapon()
{
	StopFiring();

	SetActorHiddenInGame(true);

	// Notify owner
	WeaponOwner->OnWeaponDeactivated(this);
}

void AHFWeapon::StartFiring()
{
	bIsFiring = true;

	const float TimeSinceLastShot = GetWorld()->GetTimeSeconds() - TimeOfLastShot;
	if(TimeSinceLastShot > RefireRate)
	{
		Fire();
	}
	else
	{
		if(bFullAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AHFWeapon::Fire, TimeSinceLastShot, false);
		}
	}
}

void AHFWeapon::StopFiring()
{
	bIsFiring = false;

	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

void AHFWeapon::Fire()
{
	if(!bIsFiring)
	{
		return;
	}

	FireProjectile(WeaponOwner->GetWeaponTargetLocation());

	TimeOfLastShot = GetWorld()->GetTimeSeconds();

	MakeNoise(ShotLoudness, PawnOwner, PawnOwner->GetActorLocation(), ShotNoiseRange, ShotNoiseTag);

	if(bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AHFWeapon::Fire, RefireRate, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AHFWeapon::FireCooldownExpired, RefireRate, false);
	}
}

void AHFWeapon::FireCooldownExpired()
{
	// Notify Owner
	WeaponOwner->OnSemiWeaponRefire();
}

void AHFWeapon::FireProjectile(const FVector& TargetLocation)
{
	FTransform ProjectileTransform = CalculateProjectileSpawnTransform(TargetLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = PawnOwner;

	AHFWeaponProjectile* Projectile = GetWorld()->SpawnActor<AHFWeaponProjectile>(ProjectileClass, ProjectileTransform, SpawnParams);

	WeaponOwner->PlayFiringMontage(FiringMontage);
	WeaponOwner->AddWeaponRecoil(FiringRecoil);

	if(--CurrentBullets <= 0)
	{
		CurrentBullets = MagazineSize;
	}

	WeaponOwner->UpdateWeaponHUD(CurrentBullets, MagazineSize);
}

FTransform AHFWeapon::CalculateProjectileSpawnTransform(const FVector& TargetLocation) const
{
	const FVector MuzzleLoc = FirstPersonMesh->GetSocketLocation(MuzzleSocketName);
	const FVector SpawnLoc = MuzzleLoc + ((TargetLocation - MuzzleLoc).GetSafeNormal() * MuzzleOffset);
	const FRotator AimRot = UKismetMathLibrary::FindLookAtRotation(SpawnLoc, TargetLocation + (UKismetMathLibrary::RandomUnitVector() * AimVariance));

	return FTransform(AimRot, SpawnLoc, FVector::OneVector);
}

const TSubclassOf<UAnimInstance>& AHFWeapon::GetFirstPersonAnimInstanceClass() const
{
	return FirstPersonAnimInstanceClass;
}

const TSubclassOf<UAnimInstance>& AHFWeapon::GetThirdPersonAnimInstanceClass() const
{
	return ThirdPersonAnimInstanceClass;
}
