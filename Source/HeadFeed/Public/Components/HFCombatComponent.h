// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UHFCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFCombatComponent();

	void PrimaryAttack(); // Gun
	void SecondaryAttack(); // Melee

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Head Feed|Combat|Guns")
	float GunDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Guns")
	float HeadShotMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Guns")
	float MaxRange = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Health")
	float StandardHealAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Health")
	float FinisherHealAmount = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Melee")
	float MeleeDistance = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Setup")
	float StunDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Combat|Setup")
	FName HeadBoneName = FName("head");

private:	
	void ExecuteHitscan();
	void ExecuteMeleeAction();
};
