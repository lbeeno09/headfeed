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
	void OnPlayerKilled(AController* VictimController);

protected:
	void RequestRespawn(AController* Controller);

	UPROPERTY(EditAnywhere, Category="Head Feed|Rules")
	float RespawnDelay = 5.0f;
};
