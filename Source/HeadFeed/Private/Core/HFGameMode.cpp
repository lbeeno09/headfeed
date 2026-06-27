// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/HFGameMode.h"
#include "Kismet/GameplayStatics.h"

void AHFGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AHFGameMode::GameOver()
{
	DisplayEndScreen();

	if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
