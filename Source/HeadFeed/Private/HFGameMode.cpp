// Fill out your copyright notice in the Description page of Project Settings.

#include "HFGameMode.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

void AHFGameMode::OnPlayerKilled(AController* VictimController)
{
	if(VictimController)
	{
		FTimerHandle RespawnTimer;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUObject(this, &AHFGameMode::RequestRespawn, VictimController);

		GetWorldTimerManager().SetTimer(RespawnTimer, RespawnDelegate, RespawnDelay, false);
	}
}

void AHFGameMode::RequestRespawn(AController* Controller)
{
	if(Controller)
	{
		RestartPlayer(Controller);
	}
}
