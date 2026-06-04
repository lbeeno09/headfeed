// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UHFWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFWeaponComponent();

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	virtual void Fire();
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	virtual void ThrowWeapon();
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	virtual void Reload();

	int32 GetCurrentAmmo() { return CurrentAmmo; }
	int32 GetMaxAmmo() { return MaxAmmo; }

	UPROPERTY(BlueprintAssignable, Category = "Head Feed|Combat")
	FOnAmmoChanged OnAmmoChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Stats")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Stats")
	float Range = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Stats")
	int32 CurrentAmmo = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Stats")
	int32 MaxAmmo = 12;

	//UFUNCTION(BlueprintImplementableEvent, Category = "Head Feed|Combat|Effects")
	//void PlayFireEffects();
};
