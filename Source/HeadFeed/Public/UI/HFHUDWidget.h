// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HFHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEADFEED_API UHFHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateLifeBar(float CurrentLife, float MaxLife);
	UFUNCTION()
	void UpdateAmmoDisplay(int32 CurrentAmmo, int32 MaxAmmo);

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* AmmoGrid;
};
