// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HFBulletCounterUI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class HEADFEED_API UHFBulletCounterUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Allows Blueprint to update Bullet Count
	UFUNCTION(BlueprintImplementableEvent, Category="Head Feed", meta=(DisplayName="UpdateBulletCounter"))
	void BP_UpdateBulletCounter(int32 MagazineSize, int32 BulletCount);

	// Allows Blueprint to update new life total and damage effect on HUD
	UFUNCTION(BlueprintImplementableEvent, Category = "Head Feed", meta = (DisplayName = "Damaged"))
	void BP_Damaged(float LifePercent);
};
