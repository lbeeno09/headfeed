// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonPlayClicked);
	ButtonSettings->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonSettingsClicked);
	ButtonCredits->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonCreditsClicked);
	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnButtonQuitClicked);
}

void UMainMenuWidget::OnButtonPlayClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_FlatMap"));
}

void UMainMenuWidget::OnButtonSettingsClicked()
{
	OnMenuSettingsClicked.Broadcast();
}

void UMainMenuWidget::OnButtonCreditsClicked()
{
	OnMenuCreditsClicked.Broadcast();
}

void UMainMenuWidget::OnButtonQuitClicked()
{
	if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
