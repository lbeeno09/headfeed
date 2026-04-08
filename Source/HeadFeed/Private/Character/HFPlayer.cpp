// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/HFPlayer.h"
#include "HFGameMode.h"
#include "Components/HFLifeLinkComponent.h"
#include "Weapons/HFWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AHFPlayer::AHFPlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 96.0f);
	GetCharacterMovement()->MaxWalkSpeed = 900.f;
	GetCharacterMovement()->MaxAcceleration = 5000.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 5000.0f;

	GetMesh()->SetOwnerNoSee(true);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(GetCapsuleComponent());
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	PlayerCamera->bUsePawnControlRotation = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(PlayerCamera);
	ArmMesh->bOnlyOwnerSee = true;
	ArmMesh->SetCastShadow(false);
	ArmMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	WeaponSlot = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Slot"));
	WeaponSlot->SetupAttachment(ArmMesh);

	LifeLinkComp = CreateDefaultSubobject<UHFLifeLinkComponent>(TEXT("Lifelink Component"));
}

void AHFPlayer::BeginPlay()
{
	Super::BeginPlay();

	if(StartingWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		CurrentWeapon = GetWorld()->SpawnActor<AHFWeapon>(StartingWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		if(CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(WeaponSlot, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}

	if(LifeLinkComp)
	{
		LifeLinkComp->OnDeath.AddDynamic(this, &AHFPlayer::Die);
	}
}

float AHFPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ReceiveDamage(Damage);

	return Damage;
}

void AHFPlayer::ReceiveDamage(float DamageAmount)
{
	if(LifeLinkComp)
	{
		LifeLinkComp->ModifyHealth(-DamageAmount);
	}
}

void AHFPlayer::RequestFire()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->PrimaryFire();
	}
}

void AHFPlayer::RequestStopFire()
{

}

void AHFPlayer::RequestReload()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void AHFPlayer::RequestMelee()
{

}


void AHFPlayer::Die()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	DisableInput(nullptr);

	BP_OnDeath();

	if(AHFGameMode* GM = GetWorld()->GetAuthGameMode<AHFGameMode>())
	{
		GM->OnPlayerKilled(GetController());
	}
}