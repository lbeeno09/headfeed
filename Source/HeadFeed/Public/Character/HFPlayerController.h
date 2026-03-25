// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HFPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class HEADFEED_API AHFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHFPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleStopJump();

	void HandleFire();
	void HandleStopFire();
	void HandleReload();
	void HandleMelee();


	UPROPERTY(EditAnywhere, Category="Head Feed|Input|Input Mappings")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, Category = "Head Feed|Input")
	UInputAction* MeleeAction;
};
