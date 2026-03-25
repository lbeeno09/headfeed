// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/HFPlayerController.h"
#include "Character/HFPlayerCameraManager.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

AHFPlayerController::AHFPlayerController()
{
	PlayerCameraManagerClass = AHFPlayerCameraManager::StaticClass();

	bShowMouseCursor = false;
}

void AHFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AHFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Movement
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHFPlayerController::HandleMove);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHFPlayerController::HandleLook);

		// Jump
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AHFPlayerController::HandleJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHFPlayerController::HandleStopJump);

		// Combat
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &AHFPlayerController::HandleFire);
		EIC->BindAction(FireAction, ETriggerEvent::Completed, this, &AHFPlayerController::HandleStopFire);
		EIC->BindAction(ReloadAction, ETriggerEvent::Started, this, &AHFPlayerController::HandleReload);
		EIC->BindAction(MeleeAction, ETriggerEvent::Started, this, &AHFPlayerController::HandleMelee);
	}
}

void AHFPlayerController::HandleMove(const FInputActionValue& Value)
{
	if(APawn* ControlledPawn = GetPawn())
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
	}
}

void AHFPlayerController::HandleLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AHFPlayerController::HandleJump()
{
	if(ACharacter* TargetCharacter = Cast<ACharacter>(GetPawn()))
	{
		TargetCharacter->Jump();
	}
}

void AHFPlayerController::HandleStopJump()
{
	if(ACharacter* TargetCharacter = Cast<ACharacter>(GetPawn()))
	{
		TargetCharacter->StopJumping();
	}
}

void AHFPlayerController::HandleFire()
{
	UE_LOG(LogTemp, Log, TEXT("Left Click: Fire"));
}

void AHFPlayerController::HandleStopFire()
{
	UE_LOG(LogTemp, Log, TEXT("Fire Released"));
}

void AHFPlayerController::HandleReload()
{
	UE_LOG(LogTemp, Log, TEXT("R: Reload"));
}

void AHFPlayerController::HandleMelee()
{
	UE_LOG(LogTemp, Log, TEXT("Right Click: Melee"));
}
