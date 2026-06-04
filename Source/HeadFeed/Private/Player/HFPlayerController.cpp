// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/HFPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"

void AHFPlayerController::SetupInputComponent()
{
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for(UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}

void AHFPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(IsLocalController() && HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if(HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}
