// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFLifeLinkComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, HealthPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UHFLifeLinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHFLifeLinkComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Head Feed|LifeLink")
	void ModifyHealth(float Amount, bool bIsKillHeal = false);

	UFUNCTION(BlueprintCallable, Category = "Head Feed|LifeLink")
	float GetHealthPercent() const { return (MaxHP > 0) ? (CurrentHP / MaxHP) : 0.0f; }

	UPROPERTY(BlueprintAssignable, Category="Head Feed|LifeLink|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Head Feed|LifeLink|Events")
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Head Feed|LifeLink|Stats")
	float MaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Head Feed|LifeLink|Stats")
	float CurrentHP;

	UPROPERTY(EditAnywhere, Category = "Head Feed|LifeLink|Stats")
	float DecayRate = 5.0f;

	float DecayTimerDelay = 0.0f;
};
