// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainGame/GameUIWidget.h"
#include "Player/HFPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

void UGameUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AHFPlayer* Player = Cast<AHFPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Player->GetWeaponComponent()->OnAmmoCountChanged.AddDynamic(this, &UGameUIWidget::HandleAmmoChange);
		Player->OnHealthChanged.AddDynamic(this, &UGameUIWidget::HandleHealthChange);
	}
}

void UGameUIWidget::HandleAmmoChange(int32 CurrentAmmmo, int32 MaxAmmo)
{
	if(SpawnedAmmoImages.Num() != MaxAmmo)
	{
		RefreshAmmo(MaxAmmo);
	}

	for(int32 i = 0; i < SpawnedAmmoImages.Num(); i++)
	{
		if(SpawnedAmmoImages[i])
		{
			float TargetOpacity = i < CurrentAmmmo ? 1.0f : 0.15f;
			SpawnedAmmoImages[i]->SetRenderOpacity(TargetOpacity);
		}
	}
}

void UGameUIWidget::RefreshAmmo(int32 MaxAmmo)
{
	AmmoContainer->ClearChildren();
	SpawnedAmmoImages.Empty();

	for(int32 i = 0; i < MaxAmmo; i++)
	{
		if(UImage* NewAmmoImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass()))
		{
			NewAmmoImage->SetBrushFromTexture(BulletTexture);

			if(UHorizontalBoxSlot* BoxSlot = AmmoContainer->AddChildToHorizontalBox(NewAmmoImage))
			{
				BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

				BoxSlot->SetHorizontalAlignment(HAlign_Fill);
				BoxSlot->SetVerticalAlignment(VAlign_Fill);
				BoxSlot->SetPadding(FMargin(0.0f, 0.0f, 4.0f, 0.0f));
			}
			SpawnedAmmoImages.Add(NewAmmoImage);
		}
	}
}

void UGameUIWidget::HandleHealthChange(float NewPercent)
{
	if(NewPercent <= 0.0f)
	{

	}

	ProgressBarHealth->SetPercent(NewPercent);
}