// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainGame/GameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonRetry->OnClicked.AddDynamic(this, &UGameOverWidget::OnButtonRetryClicked);
	ButtonTitle->OnClicked.AddDynamic(this, &UGameOverWidget::OnButtonTitleClicked);
}

void UGameOverWidget::HandleResultText()
{

}

void UGameOverWidget::OnButtonRetryClicked()
{
	FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentMapName));
}

void UGameOverWidget::OnButtonTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_MainMenu"));
}
