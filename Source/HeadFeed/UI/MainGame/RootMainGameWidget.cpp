// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainGame/RootMainGameWidget.h"
#include "UI/MainGame/GameUIWidget.h"
#include "UI/MainGame/SwitcherPauseWidget.h"
#include "UI/MainGame/GameOverWidget.h"
#include "Player/HFPlayer.h"
#include "Components/Overlay.h"

void URootMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AHFPlayer* Player = Cast<AHFPlayer>(GetOwningPlayerPawn()))
	{
		Player->OnPauseInputTriggered.AddDynamic(this, &URootMainGameWidget::HandlePauseInput);
		Player->OnHealthChanged.AddDynamic(this, &URootMainGameWidget::HandleGameOverSwitch);
	}

	//WBPGameUIWidget
	WBPSwitcherPause->OnPauseResumeRequested.AddDynamic(this, &URootMainGameWidget::ClosePauseMenu);
	WBPSwitcherPause->SetVisibility(ESlateVisibility::Collapsed);
	WBPGameOver->SetVisibility(ESlateVisibility::Collapsed);
}

void URootMainGameWidget::HandleGameOverSwitch(float Percent)
{
	if(Percent <= 0.0f)
	{
		WBPGameOver->SetVisibility(ESlateVisibility::Visible);

		APlayerController* PC = GetOwningPlayer();
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(WBPSwitcherPause->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->SetPause(true);
	}
}

void URootMainGameWidget::HandlePauseInput()
{
	if(bIsMenuOpen)
	{
		ClosePauseMenu();
	}
	else
	{
		bIsMenuOpen = true;
		WBPSwitcherPause->SetVisibility(ESlateVisibility::Visible);

		APlayerController* PC = GetOwningPlayer();
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(WBPSwitcherPause->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->SetPause(true);
	}
}

void URootMainGameWidget::ClosePauseMenu()
{
	bIsMenuOpen = false;
	WBPSwitcherPause->SetVisibility(ESlateVisibility::Collapsed);

	APlayerController* PC = GetOwningPlayer();
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(false);
	PC->SetPause(false);
}
