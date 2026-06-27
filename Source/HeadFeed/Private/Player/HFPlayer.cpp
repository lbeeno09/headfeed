// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/HFPlayer.h"
#include "Core/HFGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AHFPlayer::AHFPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh());
	FirstPersonCamera->bUsePawnControlRotation = true;
}

void AHFPlayer::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	bIsGameplayActive = true;
}

void AHFPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsGameplayActive)
	{
		UpdateHealth(-HealthDecayRate * DeltaTime);
	}
}

void AHFPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Turn Around
		if(LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHFPlayer::Look);
		}
		if(MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHFPlayer::Move);
		}
		if(JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AHFPlayer::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHFPlayer::StopJumping);
		}
		if(MeleeAction)
		{
			EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AHFPlayer::Melee);
		}
		if(ShootAction)
		{
			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AHFPlayer::Shoot);
		}
		if(ReloadAction)
		{
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AHFPlayer::Reload);
		}
	}
}

void AHFPlayer::UpdateHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);

	float HealthPercent = CurrentHealth / MaxHealth;
	OnHealthChanged.Broadcast(HealthPercent);

	if(CurrentHealth <= 0.0f)
	{
		if(AHFGameMode* GameMode = Cast<AHFGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->GameOver();
		}
	}
}

void AHFPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHFPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AHFPlayer::Melee()
{

}

void AHFPlayer::Shoot()
{

}

void AHFPlayer::Reload()
{

}

void AHFPlayer::StartGameplay()
{
	bIsGameplayActive = true;
}
