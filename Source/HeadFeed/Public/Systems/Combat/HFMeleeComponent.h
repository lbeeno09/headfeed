// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFMeleeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UHFMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFMeleeComponent();

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Combat")
	virtual void Melee();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Stats")
	float MeleeDamage = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Stats")
	float MeleeRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Combat|Stats")
	float SweepRadius = 40.0f;

	//UFUNCTION(BlueprintImplementableEvent, Category = "Head Feed|Combat|Effects")
	//void PlayMeleeEffects();

};
