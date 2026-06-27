// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HEADFEED_API AHFPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Input|Input Mappings")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
};
