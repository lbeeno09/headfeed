// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEADFEED_API AHFGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Head Feed|Game Flow")
	void DisplayEndScreen();

	void GameOver();

private:
	virtual void BeginPlay() override;
};
