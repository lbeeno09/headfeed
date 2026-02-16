// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HFCharacterBase.h"
#include "Weapons/HFWeaponHolder.h"
#include "HFMainCharacter.generated.h"

class AHFWeapon;
class UInputAction;
class UInputComponent;
class UPawnNoiseEmitterComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBulletCountUpdatedDelegate, int32, MagazineSize, int32, Bullets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamagedDelegate, float, LifePercent);

/**
 * 
 */
UCLASS(Abstract)
class HEADFEED_API AHFMainCharacter : public AHFCharacterBase, public IHFWeaponHolder
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;

protected:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SwitchWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrid_L");

	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrid_L");

	UPROPERTY(EditAnywhere, Category = "Aim", meta=(ClampMin=0, ClampMax=100000, Units="cm"))
	float MaxAimDistance = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHP = 500.0f;

	float CurrentHP = 0.0f;

	TArray<AHFWeapon*> OwnedWeapons;

	TObjectPtr<AHFWeapon> CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Destruction", meta=(ClampMin=0, ClampMax=10, Units="s"))
	float RespawnTime = 5.0f;

	FTimerHandle RespawnTimer;

public:
	FBulletCountUpdatedDelegate OnBulletCountUpdated;

	FDamagedDelegate OnDamaged;

public:
	AHFMainCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoStartFiring();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStopFiring();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoSwitchWeapon();

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
	AHFWeapon* FindWeaponOfType(TSubclassOf<AHFWeapon> WeaponClass) const;

	void Die();

	UFUNCTION(BlueprintImplementableEvent, Category="Shooter", meta=(DisplayName="On Death"))
	void BP_OnDeath();

	void OnRespawn();
};
