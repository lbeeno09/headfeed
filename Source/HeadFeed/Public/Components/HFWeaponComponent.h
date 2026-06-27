// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFWeaponComponent.generated.h"

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
class HEADFEED_API UHFWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFWeaponComponent();

	void StartFiring(const FVector& EyeLocation, const FVector& LookDirection);
	void StopFiring();
	void Reload();
	void EquipWeapon(const FWeaponData& NewWeaponData);
	void ThrowWeapon(const FVector& LaunchLocation, const FVector& LaunchDirection);

	UFUNCTION(BlueprintPure, Category = "Head Feed|Combat")
	FWeaponData GetCurrentWeaponStats() const { return CurrentWeapon; }

protected:
	virtual void BeginPlay() override;

	void FireExecutionLoop();
	void ResetFireCooldown();

	UPROPERTY(BlueprintReadOnly, Category = "Head Feed|Combat")
	FWeaponData CurrentWeapon;
	FWeaponData DefaultWeaponPistol;

	UPROPERTY(EditDefaultsOnly, Category = "Head Feed|Combat Setup")
	TSubclassOf<class AThrownWeapon> ThrownWeaponClass;

	FVector CachedEyeLocation;
	FVector CachedLookDirection;

	FTimerHandle FireRateTimerHandle;
	bool bCanFire = true;

};
