// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HFPlayerController.generated.h"

class UInputMappingContext;
class AHFMainCharacter;
class UHFBulletCounterUI;

/**
 * 
 */
UCLASS(Abstract)
class HEADFEED_API AHFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category="Head Feed|Respawn")
	TSubclassOf<AHFMainCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, Category = "Head Feed|UI")
	TSubclassOf<UHFBulletCounterUI> BulletCounterUIClass;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Player")
	FName PlayerPawnTag = FName("Player");

	TObjectPtr<UHFBulletCounterUI> BulletCounterUI;

protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPawnDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnBulletCountUpdated(int32 MagazineSize, int32 Bullets);

	UFUNCTION()
	void OnPawnDamaged(float LifePercent);
};
