// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/MeleeComponent.h"
#include "InputActionValue.h"
#include "HFPlayer.generated.h"

class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewPercent);

UCLASS()
class HEADFEED_API AHFPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AHFPlayer();

	void DecreaseHealth(float DamageAmount);
	void RegainHealth();

	UCameraComponent* GetCameraComponent() { return FirstPersonCamera; }
	UWeaponComponent* GetWeaponComponent() { return WeaponComponent; }
	UMeleeComponent* GetMeleeComponent() { return MeleeComponent; }

	FOnHealthChanged OnHealthChanged;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Input")
	TObjectPtr<UInputAction> JumpAction;
	// Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Input")
	TObjectPtr<UInputAction> MeleeAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Input")
	TObjectPtr<UInputAction> ShootAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Input")
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Head Feed|Input")
	TObjectPtr<UInputAction> ThrowAction;

	// TODO: Add Weapon Component 
	UPROPERTY(VisibleAnywhere, Category = "Head Feed|Components")
	TObjectPtr<class UCameraComponent> FirstPersonCamera;
	UPROPERTY(VisibleAnywhere, Category = "Head Feed|Components")
	TObjectPtr<class UWeaponComponent> WeaponComponent;
	UPROPERTY(VisibleAnywhere, Category = "Head Feed|Components")
	TObjectPtr<class UMeleeComponent> MeleeComponent;

	// Gameplay Stats
	// Game Loop 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Escape|Runtime|Health")
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Escape|Runtime|Health")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Escape|Runtime|Health")
	float HealthDecayRate = 10.0f;

private:
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Melee();
	void ShootStart();
	void ShootComplete();
	void Reload();
	void Throw();
};
