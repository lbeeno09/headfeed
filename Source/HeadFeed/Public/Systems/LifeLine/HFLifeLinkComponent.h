// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFLifeLinkComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifeChanged, float, CurrentLife, float, MaxLife);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UHFLifeLinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFLifeLinkComponent();

	UFUNCTION(BlueprintCallable, Category = "Head Feed|Life Line")
	void AddLife(float Amount);

	UPROPERTY(BlueprintAssignable, Category = "Head Feed|Life Line")
	FOnLifeChanged OnLifeChanged;

	float GetCurrentLife() { return CurrentLife; }
	float GetMaxLife() { return MaxLife; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Life Line")
	float CurrentLife = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Life Line")
	float MaxLife = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Feed|Life Line")
	float DecayRate = 2.0f;

private:
	void HandleDeath();
};
