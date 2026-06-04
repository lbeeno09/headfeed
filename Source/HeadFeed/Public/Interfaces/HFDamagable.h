// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HFDamagable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHFDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
// Add interface functions to this class. This is the class that will be inherited to implement this interface.
class HEADFEED_API IHFDamagable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Head Feed|Combat")
	void ReceiveDamage(float DamageAmount, AActor* DamageCauser);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Feed|Combat")
	float GetHealth() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Head Feed|Combat")
	float GetMaxHealth() const;
};
