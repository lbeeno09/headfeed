// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainGameHUD.generated.h"

class URootMainGameWidget;

/**
 * 
 */
UCLASS()
class HEADFEED_API AMainGameHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Head Feed|Widget")
	TSubclassOf<URootMainGameWidget> MainGameWidgetClass;

	UPROPERTY()
	TObjectPtr<URootMainGameWidget> MainGameWidgetInstance;

};
