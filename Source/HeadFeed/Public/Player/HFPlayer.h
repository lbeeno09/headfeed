// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Gameplay")
	void StartGameplay();

	void UpdateHealth(float Amount);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Head Feed | Events")
	FOnHealthChanged OnHealthChanged;

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

	// Gameplay Stats
	// Game Loop 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Escape|Runtime")
	bool bIsGameplayActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Escape|Runtime|Health")
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Escape|Runtime|Health")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Escape|Runtime|Health")
	float HealthDecayRate = 8.0f;

private:
	// TODO: Add Weapon Component 
	UPROPERTY(VisibleAnywhere, Category = "Head Feed|Components")
	TObjectPtr<class UCameraComponent> FirstPersonCamera;

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Melee();
	void Shoot();
	void Reload();

};
