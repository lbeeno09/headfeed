// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HFHUD.h"
#include "Blueprint/UserWidget.h"

void AHFHUD::BeginPlay()
{
	Super::BeginPlay();

	if(LifeLinkWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LifeLinkWidgetClass);
		if(CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
