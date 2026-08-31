// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/MainGame/MainGamePlayerController.h"
#include "UI/MainGame/RootMainGameWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AMainGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(MainGameMappingContext)
		{
			Subsystem->AddMappingContext(MainGameMappingContext, 0);
		}
	}

	if(MainGameWidgetClass)
	{
		MainGameWidgetInstance = CreateWidget<URootMainGameWidget>(this, MainGameWidgetClass);
		if(MainGameWidgetInstance)
		{
			MainGameWidgetInstance->AddToViewport(0);
		}
	}

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

void AMainGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if(PauseAction)
		{
			EIC->BindAction(PauseAction, ETriggerEvent::Started, this, &AMainGamePlayerController::TogglePause);
		}
	}
}

void AMainGamePlayerController::TogglePause()
{
	MainGameWidgetInstance->TraversePauseTree();

	UpdateInputMode(MainGameWidgetInstance->IsGameHUD());
}

void AMainGamePlayerController::UpdateInputMode(bool bIsGameHUD)
{
	if(bIsGameHUD)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(MainGameWidgetInstance->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
	}
}
