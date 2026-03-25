// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HFDamagable.h"
#include "GameFramework/Character.h"
#include "HFPlayer.generated.h"

class UHFLifeLinkComponent;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class HEADFEED_API AHFPlayer : public ACharacter, public IHFDamagable
{
	GENERATED_BODY()

public:
	AHFPlayer();

	virtual void ReceiveDamage(float DamageAmount) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Head Feed|Components")
	UHFLifeLinkComponent* LifeLinkComp;

	UFUNCTION()
	void Die();

	UFUNCTION(BlueprintImplementableEvent, Category="Head Feed|Category", meta=(DisplayName="On Death"))
	void BP_OnDeath();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;
};
