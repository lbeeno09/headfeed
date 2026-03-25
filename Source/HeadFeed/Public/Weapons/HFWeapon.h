// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HFWeapon.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);

UCLASS()
class HEADFEED_API AHFWeapon : public AActor
{
	GENERATED_BODY()

public:
	AHFWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Head Feed|Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(BlueprintAssignable, Category = "Head Feed|Weapon|Events")
	FOnAmmoChanged OnAmmoChanged;

	virtual void PrimaryFire();
	virtual void Reload();

protected:
	virtual void BeginPlay() override;
	
	void UpdateAmmoUI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Weapon|Stats")
	int32 MaxAmmo = 20;

	int32 CurrentAmmo;
};
