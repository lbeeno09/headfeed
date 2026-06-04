// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HFHUDWidget.h"
#include "Player/HFPlayer.h"
#include "Systems/LifeLine/HFLifeLinkComponent.h"
#include "Systems/Combat/HFWeaponComponent.h"
#include "Systems/Combat/HFCombatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

void UHFHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AHFPlayer* Player = Cast<AHFPlayer>(GetOwningPlayerPawn()))
	{
		if(UHFLifeLinkComponent* LifeComp = Player->GetLifeLinkComponent())
		{
			LifeComp->OnLifeChanged.AddDynamic(this, &UHFHUDWidget::UpdateLifeBar);

			UpdateLifeBar(LifeComp->GetCurrentLife(), LifeComp->GetMaxLife());
		}

		if(UHFWeaponComponent* WeaponComp = Player->FindComponentByClass<UHFWeaponComponent>())
		{
			WeaponComp->OnAmmoChanged.AddDynamic(this, &UHFHUDWidget::UpdateAmmoDisplay);

			UpdateAmmoDisplay(WeaponComp->GetCurrentAmmo(), WeaponComp->GetMaxAmmo());
		}
	}
}

void UHFHUDWidget::UpdateLifeBar(float CurrentLife, float MaxLife)
{
	if(HealthBar && MaxLife > 0.0f)
	{
		HealthBar->SetPercent(CurrentLife / MaxLife);
	}
}

void UHFHUDWidget::UpdateAmmoDisplay(int32 CurrentAmmo, int32 MaxAmmo)
{
	if(!AmmoGrid || MaxAmmo <= 0)
	{
		return;
	}

	AmmoGrid->ClearChildren();

	for(int32 i = 0; i < MaxAmmo; i++)
	{
		UImage* BulletImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if(BulletImage)
		{
			BulletImage->SetBrushTintColor(FSlateColor(FLinearColor::Green));
			
			UUniformGridSlot* GridSlot = AmmoGrid->AddChildToUniformGrid(BulletImage, 0, i);

			if(i < CurrentAmmo)
			{
				BulletImage->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				BulletImage->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}
