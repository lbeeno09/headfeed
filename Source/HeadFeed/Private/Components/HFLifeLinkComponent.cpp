// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HFLifeLinkComponent.h"

UHFLifeLinkComponent::UHFLifeLinkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHP = MaxHP;
}

void UHFLifeLinkComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;
}

void UHFLifeLinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentHP <= 0.0f || DecayRate <= 0.0f)
	{
		return;
	}

	if(DecayTimerDelay > 0.0f)
	{
		DecayTimerDelay -= DeltaTime;
	}
	else
	{
		ModifyHealth(-DecayRate * DeltaTime);
	}
}

void UHFLifeLinkComponent::ModifyHealth(float Amount, bool bIsKillHeal)
{
	if(CurrentHP <= 0.0f)
	{
		return;
	}

	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, MaxHP);

	OnHealthChanged.Broadcast(GetHealthPercent());

	if(bIsKillHeal)
	{
		DecayTimerDelay = 0.2f;
	}

	if(CurrentHP <= 0.0f)
	{
		SetComponentTickEnabled(false);
		OnDeath.Broadcast();
	}
}
