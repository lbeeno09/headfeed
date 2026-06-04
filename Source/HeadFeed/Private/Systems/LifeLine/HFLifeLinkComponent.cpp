// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/LifeLine/HFLifeLinkComponent.h"

UHFLifeLinkComponent::UHFLifeLinkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UHFLifeLinkComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLife = MaxLife;
}


void UHFLifeLinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentLife > 0.0f)
	{
		CurrentLife -= DecayRate * DeltaTime;

		OnLifeChanged.Broadcast(CurrentLife, MaxLife);

		// TEMP: Delete after ui ready
		if(GEngine)
		{
			FString HealthMsg = FString::Printf(TEXT("HP: %.2f"), CurrentLife);
			GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, HealthMsg);
		}

		if(CurrentLife <= 0.0f)
		{
			HandleDeath();
		}
	}
}

void UHFLifeLinkComponent::AddLife(float Amount)
{
	CurrentLife = FMath::Clamp(CurrentLife + Amount, 0.0f, MaxLife);

	OnLifeChanged.Broadcast(CurrentLife, MaxLife);
}

void UHFLifeLinkComponent::HandleDeath()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LIFE LINK SEVERED - PLAYER DEAD"));
	}

	// TODO: Logic for respawn and game over
}
