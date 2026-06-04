// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/HFPlayer.h"
#include "Systems/Combat/HFCombatComponent.h"
#include "Systems/LifeLine/HFLifeLinkComponent.h"
#include "Systems/Combat/HFCombatComponent.h"
#include "Systems/Combat/HFWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

AHFPlayer::AHFPlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 96.0f);

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCharacterMovement()->BrakingDecelerationFlying = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	
	HFCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HF Camera"));
	HFCamera->SetupAttachment(GetMesh(), FName("head"));
	HFCamera->bUsePawnControlRotation = true;
	HFCamera->bEnableFirstPersonFieldOfView = true;
	HFCamera->FirstPersonFieldOfView = 90.0f;
	HFCamera->bEnableFirstPersonScale = true;
	HFCamera->FirstPersonScale = 0.6f;

	CombatComponent = CreateDefaultSubobject<UHFCombatComponent>(TEXT("Combat Component"));

	LifeLinkComponent = CreateDefaultSubobject<UHFLifeLinkComponent>(TEXT("Life Link Component"));
}

void AHFPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Jumping
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AHFPlayer::DoStartJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHFPlayer::DoEndJump);

		// Movement
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHFPlayer::DoMove);

		// Look
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHFPlayer::DoAim);

		// Attacks (Hand Over impl to Actor Comp)
		EIC->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &AHFPlayer::HandlePrimaryAttack);
		EIC->BindAction(SecondaryAttackAction, ETriggerEvent::Started, this, &AHFPlayer::HandleSecondaryAttack);

		// Reload (Hand Over impl to Actor Comp)
		EIC->BindAction(ReloadAction, ETriggerEvent::Started, this, &AHFPlayer::HandleReload);
	}

	// Heal Debug
	PlayerInputComponent->BindKey(EKeys::H, IE_Pressed, this, &AHFPlayer::HandleDebugHeal);
	// Switch Weapon State Debug
	PlayerInputComponent->BindKey(EKeys::X, IE_Pressed, this, &AHFPlayer::HandleDebugStateSwitch);
	// Damage Debug
	PlayerInputComponent->BindKey(EKeys::B, IE_Pressed, this, &AHFPlayer::HandleDebugDamage);
}

void AHFPlayer::DoStartJump()
{
	Jump();
}

void AHFPlayer::DoEndJump()
{
	StopJumping();
}

void AHFPlayer::DoMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.X);
	AddMovementInput(GetActorRightVector(), MovementVector.Y);
}

void AHFPlayer::DoAim(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AHFPlayer::HandlePrimaryAttack()
{
	GetCombatComponent()->PrimaryAttack();
}

void AHFPlayer::HandleSecondaryAttack()
{
	GetCombatComponent()->SecondaryAttack();
}

void AHFPlayer::HandleReload()
{
	if(UHFWeaponComponent* Weapon = GetCombatComponent()->GetLeftHandWeapon())
	{
		Weapon->Reload();
	}
}

void AHFPlayer::HandleDebugHeal()
{
	if(LifeLinkComponent)
	{
		float HealAmount = 20.0f;
		LifeLinkComponent->AddLife(HealAmount);

		if(GEngine)
		{
			FString Message = FString::Printf(TEXT("Healing %f"), HealAmount);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, Message);
		}
	}
}

void AHFPlayer::HandleDebugStateSwitch()
{
	SetCombatState(EHFCombatState::DoubleHanding);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("State Switched to Doule Handed"));
	}
}

void AHFPlayer::HandleDebugDamage()
{
	GetLifeLinkComponent()->AddLife(-10);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Taken damage!: HP: %.2f / %.2f"), GetLifeLinkComponent()->GetCurrentLife(), GetLifeLinkComponent()->GetMaxLife()));
	}
}

void AHFPlayer::RewardLife(float Amount)
{
	if(LifeLinkComponent)
	{
		LifeLinkComponent->AddLife(Amount);

		// TODO: Heal fx
	}
}

void AHFPlayer::ReceiveDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if(LifeLinkComponent)
	{
		LifeLinkComponent->AddLife(-DamageAmount);
	}
}

