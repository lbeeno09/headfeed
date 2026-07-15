// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/MainGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMainGameMode::GameOver_Implementation()
{
	if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
