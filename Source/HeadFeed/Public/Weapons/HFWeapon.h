// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HFWeapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class HEADFEED_API AHFWeapon : public AActor
{
	GENERATED_BODY()

public:
	AHFWeapon();

	virtual void PrimaryFire();
	virtual void Reload();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Head Feed|Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Stats")
	float DamageValue = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Stats")
	float AttackRange = 10000.0f;
};
