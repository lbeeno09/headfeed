// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/HFNPC.h"
#include "Weapons/HFWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

void AHFNPC::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Weapon = GetWorld()->SpawnActor<AHFWeapon>(WeaponClass, GetActorTransform(), SpawnParams);
}

void AHFNPC::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

float AHFNPC::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(bIsDead)
	{
		return 0.0f;
	}

	CurrentHP -= Damage;
	if(CurrentHP <= 0.0f)
	{
		Die();
	}

	return Damage;
}

void AHFNPC::AttachWeaponMeshes(AHFWeapon* WeaponToAttach)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	WeaponToAttach->AttachToActor(this, AttachmentRule);
	WeaponToAttach->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	WeaponToAttach->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
}

void AHFNPC::PlayFiringMontage(UAnimMontage* Montage)
{

}

void AHFNPC::AddWeaponRecoil(float Recoil)
{

}

void AHFNPC::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{

}

FVector AHFNPC::GetWeaponTargetLocation()
{
	const FVector AimSource = GetFirstPersonCameraComponent()->GetComponentLocation();

	FVector AimDir, AimTarget = FVector::ZeroVector;

	if(CurrentAimTarget)
	{
		AimTarget = CurrentAimTarget->GetActorLocation();
		AimTarget.Z += FMath::RandRange(MinAimOffsetZ, MaxAimOffsetZ);

		AimDir = (AimTarget - AimSource).GetSafeNormal();
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(AimDir, AimVarianceHalfAngle);
	}
	else
	{
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetFirstPersonCameraComponent()->GetForwardVector(), AimVarianceHalfAngle);
	}

	AimTarget = AimSource + (AimDir * AimRange);

	FHitResult OutHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, AimSource, AimTarget, ECC_Visibility, QueryParams);

	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AHFNPC::AddWeaponClass(const TSubclassOf<AHFWeapon>& InWeaponClass)
{

}

void AHFNPC::OnWeaponActivated(AHFWeapon* InWeapon)
{

}

void AHFNPC::OnWeaponDeactivated(AHFWeapon* InWeapon)
{

}

void AHFNPC::OnSemiWeaponRefire()
{
	if(bIsShooting)
	{
		Weapon->StartFiring();
	}
}

void AHFNPC::Die()
{
	if(bIsDead)
	{
		return;
	}

	bIsDead = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->StopActiveMovement();

	GetMesh()->SetCollisionProfileName(RagdollCollisionProfile);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetPhysicsBlendWeight(1.0f);

	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AHFNPC::DeferredDestruction, DeferredDestructionTime, false);
}

void AHFNPC::DeferredDestruction()
{
	Destroy();
}

void AHFNPC::StartShooting(AActor* ActorToShoot)
{
	CurrentAimTarget = ActorToShoot;

	bIsShooting = true;

	Weapon->StartFiring();
}

void AHFNPC::StopShooting()
{
	bIsShooting = false;

	Weapon->StopFiring();
}
