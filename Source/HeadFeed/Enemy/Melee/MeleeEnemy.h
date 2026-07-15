// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyStats.h"
#include "MeleeEnemy.generated.h"

class UMeleeComponent;

UCLASS()
class HEADFEED_API AMeleeEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AMeleeEnemy();

	UFUNCTION(BlueprintPure)
	EEnemyState GetCurrentState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable)
	UMeleeComponent* GetMeleeComponent() const { return MeleeComponent; }
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
	TObjectPtr<UMeleeComponent> MeleeComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components")
	TObjectPtr<class UWidgetComponent> HealthBarWidget;

	// NOTE: used when stunned phase is added
	bool bCanFinish = false;

private:
	void HandleDeath(AActor* Killer);
};
