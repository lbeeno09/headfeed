// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainGamePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UUserWidget;
class URootMainGameWidget;

/**
 * 
 */
UCLASS()
class HEADFEED_API AMainGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateInputMode(bool bIsGameHUD);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TObjectPtr<UInputMappingContext> MainGameMappingContext;
	UFUNCTION(BlueprintCallable, Category = "Input|Input Action")
	void TogglePause();
	UPROPERTY(EditAnywhere, Category = "Input|Input Action")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainGameWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<URootMainGameWidget> MainGameWidgetInstance;
};
