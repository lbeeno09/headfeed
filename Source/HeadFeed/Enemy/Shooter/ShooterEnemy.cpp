// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shooter/ShooterEnemy.h"
#include "Player/HFPlayer.h"
#include "Components/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AShooterEnemy::AShooterEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawAtDesiredSize(true);
}

void AShooterEnemy::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentState = EEnemyState::Active;

	if(AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if(UBlackboardComponent* BB = AIC->GetBlackboardComponent())
		{
			BB->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(EEnemyState::Active));
			BB->SetValueAsVector(TEXT("StartLocation"), GetActorLocation());
			BB->SetValueAsFloat(TEXT("FireRate"), WeaponComponent->GetCurrentWeaponStats().FireRate);
		}
	}
}

float AShooterEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(CurrentState == EEnemyState::Dead)
	{
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
	if(CurrentHealth <= 0.0f)
	{
		HandleDeath(DamageCauser);
	}

	return ActualDamage;
}

void AShooterEnemy::HandleDeath(AActor* Killer)
{
	CurrentState = EEnemyState::Dead;

	if(WeaponComponent)
	{
		WeaponComponent->StopFiring();
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();

	if(Killer)
	{
		Cast<AHFPlayer>(Killer)->RegainHealth();
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString("Shooter Dead!"));

	Destroy();
}
