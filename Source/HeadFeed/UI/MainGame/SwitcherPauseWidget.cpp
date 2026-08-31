// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainGame/SwitcherPauseWidget.h"
#include "UI/MainGame/PauseWidget.h"
#include "UI/Common/SettingsWidget.h"
#include "Components/WidgetSwitcher.h"

void USwitcherPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WBPPause->OnPauseResumeClicked.AddDynamic(this, &USwitcherPauseWidget::HandleResumeSignal);
	WBPPause->OnPauseSettingsClicked.AddDynamic(this, &USwitcherPauseWidget::SwitchToSettingsView);
	WBPSettings->OnSettingsReturnClicked.AddDynamic(this, &USwitcherPauseWidget::SwitchToPauseView);

	SwitchToPauseView();
}

void USwitcherPauseWidget::TraversePauseTree()
{
	if(WidgetSwitcherPause->GetActiveWidgetIndex() == 0)
	{
		// stub
	}
	else if(WidgetSwitcherPause->GetActiveWidgetIndex() == 1)
	{
		WidgetSwitcherPause->SetActiveWidgetIndex(EPauseIndex::Pause);
	}
}

bool USwitcherPauseWidget::IsPauseMenu()
{
	return WidgetSwitcherPause->GetActiveWidgetIndex() == 0;
}

void USwitcherPauseWidget::HandleResumeSignal()
{
	OnPauseResumeRequested.Broadcast();
}

void USwitcherPauseWidget::SwitchToPauseView()
{
	WidgetSwitcherPause->SetActiveWidgetIndex(EPauseIndex::Pause);
}

void USwitcherPauseWidget::SwitchToSettingsView()
{
	WidgetSwitcherPause->SetActiveWidgetIndex(EPauseIndex::Settings);
}
