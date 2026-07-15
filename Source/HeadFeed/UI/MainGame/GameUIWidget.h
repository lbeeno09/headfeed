// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUIWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class HEADFEED_API UGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> AmmoContainer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI Setup")
	TObjectPtr<class UTexture2D> BulletTexture;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBarHealth;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ImageReticle;

private:
	UPROPERTY()
	TArray<TObjectPtr<class UImage>> SpawnedAmmoImages;

	void RefreshAmmo(int32 MaxAmmo);

	UFUNCTION()
	void HandleAmmoChange(int32 CurrentAmmmo, int32 MaxAmmo);
	UFUNCTION()
	void HandleHealthChange(float NewPercent);
};
