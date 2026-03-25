// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HFCombatInterface.h"
#include "HFShooter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPawnDeathDelegate);

class UHFCombatComponent;

UCLASS()
class HEADFEED_API AHFShooter : public ACharacter, public IHFCombatInterface
{
	GENERATED_BODY()

public:
	AHFShooter();

	virtual bool IsStunned() const override { return bIsStunned; }
	virtual void ExecuteFinisher() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UHFCombatComponent* GetCombatComp() const { return CombatComp; }

	UPROPERTY(BlueprintAssignable, Category="Head Feed|Events")
	FPawnDeathDelegate OnPawnDeath;

protected:
	virtual void BeginPlay() override;

	void Die();
	void OnStunTimeout();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Head Feed|Components")
	UHFCombatComponent* CombatComp;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Stats")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Damage")
	FName RagdollCollisionProfile = FName("Ragdoll");

	float CurrentHP;
	bool bIsStunned = false;
	bool bIsDead = false;

	FTimerHandle StunTimer;
};
