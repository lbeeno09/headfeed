// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/Combat/HFCombatComponent.h"
#include "Player/HFPlayer.h"

UHFCombatComponent::UHFCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	LeftHandWeapon = CreateDefaultSubobject<UHFWeaponComponent>(TEXT("Left Hand Weapon"));
	RightHandMelee = CreateDefaultSubobject<UHFMeleeComponent>(TEXT("Right Hand Melee"));
}


void UHFCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UHFCombatComponent::PrimaryAttack()
{
	if(LeftHandWeapon)
	{
		LeftHandWeapon->Fire();
	}
}

void UHFCombatComponent::SecondaryAttack()
{
	AHFPlayer* Player = Cast<AHFPlayer>(GetOwner());
	if(!Player)
	{
		return;
	}

	if(Player->GetCombatState() == EHFCombatState::SingleHandMelee)
	{
		if(RightHandMelee)
		{
			RightHandMelee->Melee();
		}
	}
	else if(Player->GetCombatState() == EHFCombatState::DoubleHanding)
	{
		if(LeftHandWeapon)
		{
			LeftHandWeapon->ThrowWeapon();
		}
	}
	
}
