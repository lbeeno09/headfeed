// Fill out your copyright notice in the Description page of Project Settings.

#include "HFPlayerController.h"
#include "HFMainCharacter.h"
#include "UI/HFBulletCounterUI.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void AHFPlayerController::SetupInputComponent()
{
	if(IsLocalPlayerController())
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for(UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}

void AHFPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InPawn->OnDestroyed.AddDynamic(this, &AHFPlayerController::OnPawnDestroyed);

	if(AHFMainCharacter* MainCharacter = Cast<AHFMainCharacter>(InPawn))
	{
		MainCharacter->Tags.Add(PlayerPawnTag);

		MainCharacter->OnBulletCountUpdated.AddDynamic(this, &AHFPlayerController::OnBulletCountUpdated);
		MainCharacter->OnDamaged.AddDynamic(this, &AHFPlayerController::OnPawnDamaged);

		MainCharacter->OnDamaged.Broadcast(1.0f);
	}
}

void AHFPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
	BulletCounterUI->BP_UpdateBulletCounter(0, 0);

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);
	if(ActorList.Num() > 0)
	{
		AActor* RandomPlayerStart = ActorList[FMath::RandRange(0, ActorList.Num() - 1)];

		const FTransform SpawnTransform = RandomPlayerStart->GetActorTransform();
		if(AHFMainCharacter* RespawnedCharacter = GetWorld()->SpawnActor<AHFMainCharacter>(CharacterClass, SpawnTransform))
		{
			Possess(RespawnedCharacter);
		}
	}
}

void AHFPlayerController::OnBulletCountUpdated(int32 MagazineSize, int32 Bullets)
{
	if(BulletCounterUI)
	{
		BulletCounterUI->BP_UpdateBulletCounter(MagazineSize, Bullets);
	}
}

void AHFPlayerController::OnPawnDamaged(float LifePercent)
{
	if(IsValid(BulletCounterUI))
	{
		BulletCounterUI->BP_Damaged(LifePercent);
	}
}
