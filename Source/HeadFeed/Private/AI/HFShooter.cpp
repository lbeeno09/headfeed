// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/HFShooter.h"
#include "Components/HFCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AHFShooter::AHFShooter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.0f, 96.0f);

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	CombatComp = CreateDefaultSubobject<UHFCombatComponent>(TEXT("Combat Component"));
}

void AHFShooter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
}

float AHFShooter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(bIsDead || bIsStunned) 
	{
		return 0.0f;
	}

	if(Damage > 500.0f)
	{
		bIsStunned = true;

		GetWorldTimerManager().SetTimer(StunTimer, this, &AHFShooter::OnStunTimeout, 2.0f, false);

		return Damage;
	}

	CurrentHP -= Damage;
	if(CurrentHP <= 0.0f)
	{
		Die();
	}

	return Damage;
}

void AHFShooter::ExecuteFinisher()
{
	GetWorldTimerManager().ClearTimer(StunTimer);

	Die();
}

void AHFShooter::OnStunTimeout()
{
	Die();
}

void AHFShooter::Die()
{
	if(bIsDead)
	{
		return;
	}
	bIsDead = true;

	OnPawnDeath.Broadcast();

	// disable capsule collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();

	// Enable ragdoll
	GetMesh()->SetCollisionProfileName(RagdollCollisionProfile);
	GetMesh()->SetSimulatePhysics(true);
}
