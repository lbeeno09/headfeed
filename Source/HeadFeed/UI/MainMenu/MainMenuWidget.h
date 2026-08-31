// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuSettingsClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuCreditsClicked);

class UButton;

/**
 * 
 */
UCLASS()
class HEADFEED_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnMenuSettingsClicked OnMenuSettingsClicked;
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnMenuCreditsClicked OnMenuCreditsClicked;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanelMenu;
	UFUNCTION()
	void OnButtonPlayClicked();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonPlay;
	UFUNCTION()
	void OnButtonSettingsClicked();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonSettings;
	UFUNCTION()
	void OnButtonCreditsClicked();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonCredits;
	UFUNCTION()
	void OnButtonQuitClicked();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonQuit;
};
