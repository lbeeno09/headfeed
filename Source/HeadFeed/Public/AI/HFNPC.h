// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HFCharacterBase.h"
#include "Weapons/HFWeaponHolder.h"
#include "HFNPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPawnDeathDelegate);

class AHFWeapon;

/**
 * 
 */
UCLASS(Abstract)
class HEADFEED_API AHFNPC : public AHFCharacterBase, public IHFWeaponHolder
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float CurrentHP = 100.0f;

protected:
	UPROPERTY(EditAnywhere, Category="Damage")
	FName RagdollCollisionProfile = FName("Ragdoll");

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DeferredDestructionTime = 5.0f;

	TObjectPtr<AHFWeapon> Weapon;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AHFWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrid_R");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrid_R");

	UPROPERTY(EditAnywhere, Category="Aim")
	float AimRange = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float AimVarianceHalfAngle = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float MinAimOffsetZ = -35.0f;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float MaxAimOffsetZ = -60.0f;

	TObjectPtr<AActor> CurrentAimTarget;

	bool bIsShooting = false;

	bool bIsDead = false;

	FTimerHandle DeathTimer;

public:
	FPawnDeathDelegate OnPawnDeath;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Begin IHFWeaponHolder Interface
	virtual void AttachWeaponMeshes(AHFWeapon* Weapon) override;

	virtual void PlayFiringMontage(UAnimMontage* Montage) override;

	virtual void AddWeaponRecoil(float Recoil) override;

	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) override;

	virtual FVector GetWeaponTargetLocation() override;

	virtual void AddWeaponClass(const TSubclassOf<AHFWeapon>& WeaponClass) override;

	virtual void OnWeaponActivated(AHFWeapon* Weapon) override;

	virtual void OnWeaponDeactivated(AHFWeapon* Weapon) override;

	virtual void OnSemiWeaponRefire() override;
	// End IHFWeaponHolder Interface

protected:
	void Die();

	void DeferredDestruction();

public:
	void StartShooting(AActor* ActorToShoot);

	void StopShooting();
};
