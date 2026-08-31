// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/HFPlayer.h"
#include "Core/MainGame/MainGameMode.h"
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

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	MeleeComponent = CreateDefaultSubobject<UMeleeComponent>(TEXT("MeleeComponent"));
}

void AHFPlayer::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void AHFPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DecreaseHealth(HealthDecayRate * DeltaTime);
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
			EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AHFPlayer::Melee);
		}
		if(ShootAction)
		{
			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AHFPlayer::ShootStart);

			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Canceled, this, &AHFPlayer::ShootComplete);
			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AHFPlayer::ShootComplete);
		}
		if(ReloadAction)
		{
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AHFPlayer::Reload);
		}
		if(ThrowAction)
		{
			EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &AHFPlayer::Throw);
		}
	}
}

float AHFPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DecreaseHealth(ActualDamage);

	return ActualDamage;
}

void AHFPlayer::DecreaseHealth(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	float HealthPercent = CurrentHealth / MaxHealth;
	OnHealthChanged.Broadcast(HealthPercent);

	if(CurrentHealth <= 0.0f)
	{
		if(AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->GameOver_Implementation();
		}
	}
}

void AHFPlayer::RegainHealth()
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthDecayRate * 1.5f, 0.0f, MaxHealth);
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
	MeleeComponent->Attack();
}

void AHFPlayer::ShootStart()
{
	WeaponComponent->StartFiring();
}

void AHFPlayer::ShootComplete()
{
	WeaponComponent->StopFiring();
}

void AHFPlayer::Reload()
{
	WeaponComponent->Reload();
}

void AHFPlayer::Throw()
{
	WeaponComponent->ThrowWeapon();
}
