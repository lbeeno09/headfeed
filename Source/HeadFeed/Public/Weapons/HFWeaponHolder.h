// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HFWeaponHolder.generated.h"

class AHFWeapon;
class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHFWeaponHolder : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HEADFEED_API IHFWeaponHolder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttachWeaponMeshes(AHFWeapon* Weapon) = 0;

	virtual void PlayFiringMontage(UAnimMontage* Montage) = 0;

	virtual void AddWeaponRecoil(float Recoil) = 0;

	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) = 0;

	virtual FVector GetWeaponTargetLocation() = 0;

	virtual void AddWeaponClass(const TSubclassOf<AHFWeapon>& WeaponClass) = 0;

	virtual void OnWeaponActivated(AHFWeapon *Weapon) = 0;

	virtual void OnWeaponDeactivated(AHFWeapon* Weapon) = 0;

	virtual void OnSemiWeaponRefire() = 0;
};
