// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Combat/HFWeaponComponent.h"
#include "Systems/Combat/HFMeleeComponent.h"
#include "Components/ActorComponent.h"
#include "HFCombatComponent.generated.h"

class UHFWeaponComponent;
class UHFMeleeComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UHFCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFCombatComponent();

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	void PrimaryAttack();

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	void SecondaryAttack();

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Component")
	UHFWeaponComponent* GetLeftHandWeapon() { return LeftHandWeapon; }
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Component")
	UHFMeleeComponent* GetRightHandMelee() { return RightHandMelee; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Combat")
	UHFWeaponComponent* LeftHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Combat")
	UHFMeleeComponent* RightHandMelee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Assets")
	TSubclassOf<AActor> ThrownWeaponClass;
};
