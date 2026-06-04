// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HFPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class HEADFEED_API AHFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Head Feed|UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY()
	UUserWidget* HUDWidgetInstance;
};
