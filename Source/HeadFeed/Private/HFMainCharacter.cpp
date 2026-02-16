// Fill out your copyright notice in the Description page of Project Settings.

#include "HFMainCharacter.h"
#include "Weapons/HFWeapon.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"

AHFMainCharacter::AHFMainCharacter()
{
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Pawn Noise Emitter"));

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
}

void AHFMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	OnDamaged.Broadcast(1.0f);
}

void AHFMainCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AHFMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AHFMainCharacter::DoStartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AHFMainCharacter::DoStopFiring);

		// Switch weapon
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &AHFMainCharacter::DoSwitchWeapon);

	}
}

float AHFMainCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHP -= Damage;
	if(CurrentHP <= 0.0f)
	{
		Die();
	}

	OnDamaged.Broadcast(FMath::Max(0.0f, CurrentHP / MaxHP));

	return Damage;
}

void AHFMainCharacter::DoStartFiring()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StartFiring();
	}
}

void AHFMainCharacter::DoStopFiring()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}
}

void AHFMainCharacter::DoSwitchWeapon()
{
	if(OwnedWeapons.Num() > 1)
	{
		CurrentWeapon->DeactivateWeapon();

		int32 WeaponIndex = OwnedWeapons.Find(CurrentWeapon);
		if(WeaponIndex == OwnedWeapons.Num() - 1)
		{
			WeaponIndex = 0;
		}
		else
		{
			++WeaponIndex;
		}

		CurrentWeapon = OwnedWeapons[WeaponIndex];
		CurrentWeapon->ActivateWeapon();
	}
}

void AHFMainCharacter::AttachWeaponMeshes(AHFWeapon* Weapon)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	Weapon->AttachToActor(this, AttachmentRule);
	Weapon->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
}

void AHFMainCharacter::PlayFiringMontage(UAnimMontage* Montage)
{

}

void AHFMainCharacter::AddWeaponRecoil(float Recoil)
{
	AddControllerPitchInput(Recoil);
}

void AHFMainCharacter::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	OnBulletCountUpdated.Broadcast(MagazineSize, CurrentAmmo);
}

FVector AHFMainCharacter::GetWeaponTargetLocation()
{
	FHitResult OutHit;

	const FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector End = Start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AHFMainCharacter::AddWeaponClass(const TSubclassOf<AHFWeapon>& WeaponClass)
{
	AHFWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);
	if(!OwnedWeapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

		AHFWeapon* AddedWeapon = GetWorld()->SpawnActor<AHFWeapon>(WeaponClass, GetActorTransform(), SpawnParams);
		if(AddedWeapon)
		{
			OwnedWeapons.Add(AddedWeapon);
			if(CurrentWeapon)
			{
				CurrentWeapon->DeactivateWeapon();
			}

			CurrentWeapon = AddedWeapon;
			CurrentWeapon->ActivateWeapon();
		}
	}
}

void AHFMainCharacter::OnWeaponActivated(AHFWeapon* Weapon)
{
	OnBulletCountUpdated.Broadcast(Weapon->GetMagazineSize(), Weapon->GetBulletCount());

	GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
}

void AHFMainCharacter::OnWeaponDeactivated(AHFWeapon* Weapon)
{

}

void AHFMainCharacter::OnSemiWeaponRefire()
{

}

AHFWeapon* AHFMainCharacter::FindWeaponOfType(TSubclassOf<AHFWeapon> WeaponClass) const
{
	for(AHFWeapon* Weapon : OwnedWeapons)
	{
		if(Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	return nullptr;
}

void AHFMainCharacter::Die()
{
	if(IsValid(CurrentWeapon))
	{
		CurrentWeapon->DeactivateWeapon();
	}

	GetCharacterMovement()->StopMovementImmediately();
	DisableInput(nullptr);

	OnBulletCountUpdated.Broadcast(0, 0);

	BP_OnDeath();

	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AHFMainCharacter::OnRespawn, RespawnTime, false);
}

void AHFMainCharacter::OnRespawn()
{
	Destroy();
}
