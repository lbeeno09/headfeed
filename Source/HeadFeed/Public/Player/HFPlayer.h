// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HFDamagable.h"
#include "Systems/LifeLine/HFLifeLinkComponent.h"
#include "GameFramework/Character.h"
#include "HFPlayer.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputComponent;
class UInputMappingContext;
class UCameraComponent;

class UHFCombatComponent;
class UHFLifeLinkComponent;

UENUM(BlueprintType)
enum class EHFCombatState : uint8
{
	SingleHandMelee UMETA(DisplayName = "Single Hand + Melee"),
	DoubleHanding UMETA(DisplayName = "Double Handed Weapon")
};

UCLASS(Abstract, Blueprintable)
class HEADFEED_API AHFPlayer : public ACharacter, public IHFDamagable
{
	GENERATED_BODY()

public:
	AHFPlayer();

	// IHFDamagable Interface
	virtual void ReceiveDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual float GetHealth_Implementation() const override { return LifeLinkComponent->GetCurrentLife(); }
	virtual float GetMaxHealth_Implementation() const override { return LifeLinkComponent->GetMaxLife(); }
	// IHFDamagable Interface

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Camera")
	UCameraComponent* GetHFCamera() const { return HFCamera; }
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	EHFCombatState GetCombatState() const { return CurrentCombatState; }
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	UHFCombatComponent* GetCombatComponent() const { return CombatComponent; }
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Life Line")
	UHFLifeLinkComponent* GetLifeLinkComponent() const { return LifeLinkComponent; }

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	void SetCombatState(EHFCombatState NewState) { CurrentCombatState = NewState; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Line Line")
	void RewardLife(float Amount);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components")
	UCameraComponent* HFCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components")
	UHFCombatComponent* CombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components")
	UHFLifeLinkComponent* LifeLinkComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Combat")
	EHFCombatState CurrentCombatState = EHFCombatState::SingleHandMelee;

protected:
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Input")
	virtual void DoStartJump();
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Input")
	virtual void DoEndJump();
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Input")
	virtual void DoMove(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Head Feed|Input")
	virtual void DoAim(const FInputActionValue& Value);

	void HandlePrimaryAttack();
	void HandleSecondaryAttack();
	void HandleReload();

	void HandleDebugHeal();
	void HandleDebugStateSwitch();
	void HandleDebugDamage();

	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* PrimaryAttackAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* SecondaryAttackAction;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* ReloadAction;
};
