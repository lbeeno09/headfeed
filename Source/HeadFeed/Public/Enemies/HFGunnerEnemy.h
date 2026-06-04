// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HFDamagable.h"
#include "GameFramework/Character.h"
#include "HFGunnerEnemy.generated.h"

UCLASS()
class HEADFEED_API AHFGunnerEnemy : public ACharacter, public IHFDamagable
{
	GENERATED_BODY()

public:
	AHFGunnerEnemy();

	// Damagable Interface
	virtual void ReceiveDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual float GetHealth_Implementation() const override { return CurrentHealth; }
	virtual float GetMaxHealth_Implementation() const override { return MaxHealth; }
	// Damagable Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Feed|Combat")
	void Attack(AActor* Target);
	virtual void Attack_Implementation(AActor* Target);

	float GetAttackRange() const { return AttackRange; }

protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath(AActor* Killer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Stats")
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Head Feed|Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Stats")
	float AttackRange = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components")
	class UWidgetComponent* HealthWidget;
};
