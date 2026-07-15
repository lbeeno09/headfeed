// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoCountChanged, int32, CurrentAmmo, int32, MaxAmmo);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol,
	Deagle,
	SMG,
	Shotgun
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 MaxAmmoCount = 16;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 CurrentAmmoCount = 16;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float FireRate = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	bool bIsAutomatic = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float BaseDamage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float StunDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float MaxRange = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	class UStaticMesh* WeaponWorldMesh = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

	UFUNCTION(BlueprintCallable)
	void StartFiring();
	UFUNCTION(BlueprintCallable)
	void StopFiring();
	UFUNCTION(BlueprintCallable)
	void Reload();
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(const FWeaponData& NewWeaponData);
	UFUNCTION(BlueprintCallable)
	void ThrowWeapon();

	FOnAmmoCountChanged OnAmmoCountChanged;

	UFUNCTION(BlueprintPure, Category = "Head Feed|Combat")
	FWeaponData GetCurrentWeaponStats() const { return CurrentWeapon; }

protected:
	virtual void BeginPlay() override;

	void FireExecution();
	void ResetFireCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head Feed|Combat")
	FWeaponData CurrentWeapon;
	FWeaponData DefaultWeapon; // Pistol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat")
	float SpreadDegrees = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Head Feed|Combat Setup")
	TSubclassOf<class AThrownWeapon> ThrownWeaponClass;

	FTimerHandle FireRateTimerHandle;
	bool bCanFire = true;

};
