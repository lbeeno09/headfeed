// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainGame/RootMainGameWidget.h"
#include "UI/MainGame/GameUIWidget.h"
#include "UI/MainGame/SwitcherPauseWidget.h"
#include "UI/MainGame/GameOverWidget.h"
#include "Player/HFPlayer.h"
#include "Core/MainGame/MainGamePlayerController.h"
#include "Components/WidgetSwitcher.h"

void URootMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AHFPlayer* Player = Cast<AHFPlayer>(GetOwningPlayerPawn()))
	{
		Player->OnHealthChanged.AddDynamic(this, &URootMainGameWidget::HandleGameOver);
	}

	//WBPGameUIWidget
	WBPSwitcherPause->OnPauseResumeRequested.AddDynamic(this, &URootMainGameWidget::HandleResume);
}

void URootMainGameWidget::TraversePauseTree()
{
	if(WidgetSwitcherRoot->GetActiveWidgetIndex() == 0)
	{
		WidgetSwitcherRoot->SetActiveWidgetIndex(1);
	}
	else if(WidgetSwitcherRoot->GetActiveWidgetIndex() == 1)
	{
		if(WBPSwitcherPause->IsPauseMenu())
		{
			WidgetSwitcherRoot->SetActiveWidgetIndex(0);
		}

		WBPSwitcherPause->TraversePauseTree();
	}
}

bool URootMainGameWidget::IsGameHUD()
{
	return WidgetSwitcherRoot->GetActiveWidgetIndex() == 0;
}

void URootMainGameWidget::HandleResume()
{
	WidgetSwitcherRoot->SetActiveWidgetIndex(0);

	AMainGamePlayerController* MainController = Cast<AMainGamePlayerController>(GetOwningPlayer());
	if(MainController)
	{
		MainController->UpdateInputMode(true);
	}
}

void URootMainGameWidget::HandleGameOver(float Percent)
{
	if(Percent <= 0.0f)
	{
		WidgetSwitcherRoot->SetActiveWidgetIndex(2);

		AMainGamePlayerController* MainController = Cast<AMainGamePlayerController>(GetOwningPlayer());
		if(MainController)
		{
			MainController->UpdateInputMode(false);
		}
	}
}
