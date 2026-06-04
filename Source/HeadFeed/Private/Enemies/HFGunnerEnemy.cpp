// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/HFGunnerEnemy.h"
#include "Systems/LifeLine/HFLifeLinkComponent.h"
#include "Components/WidgetComponent.h"

AHFGunnerEnemy::AHFGunnerEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
	HealthWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHFGunnerEnemy::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void AHFGunnerEnemy::ReceiveDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if(CurrentHealth <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	if(CurrentHealth <= 0.0f)
	{
		HandleDeath(DamageCauser);
	}
}

void AHFGunnerEnemy::Attack_Implementation(AActor* Target)
{
	// Base class has no specific attack
}

void AHFGunnerEnemy::HandleDeath(AActor* Killer)
{
	if(Killer)
	{
		UHFLifeLinkComponent* PlayerLifeLink = Killer->FindComponentByClass<UHFLifeLinkComponent>();
		if(PlayerLifeLink)
		{
			// TODO: make this a header property
			float KillReward = 20.0f;
			PlayerLifeLink->AddLife(KillReward);
		}
	}

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%s has been eliminated"), *GetName()));
	}

	Destroy();
}
