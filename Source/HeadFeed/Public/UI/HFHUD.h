// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HFHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class HEADFEED_API AHFHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditANywhere, Category="Head Feed|UI")
	TSubclassOf<UUserWidget> LifeLinkWidgetClass;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UUserWidget* CurrentWidget;
};
