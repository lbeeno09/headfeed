// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyStats.h"
#include "ShooterEnemy.generated.h"

class UWeaponComponent;

UCLASS()
class HEADFEED_API AShooterEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterEnemy();

	UFUNCTION(BlueprintPure)
	EEnemyState GetCurrentState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable)
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const { return CurrentHealth / MaxHealth; }

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Vitals")
	float MaxHealth = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Head Feed|Vitals")
	float CurrentHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Head Feed|State")
	EEnemyState CurrentState = EEnemyState::Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components")
	TObjectPtr<UWeaponComponent> WeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components")
	TObjectPtr<class UWidgetComponent> HealthBarWidget;

	// NOTE: used when stunned phase is added
	bool bCanFinish = false;

private:
	void HandleDeath(AActor* Killer);
};
