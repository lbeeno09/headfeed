// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RootMainGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEADFEED_API URootMainGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> OverlayRoot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGameUIWidget> WBPGameUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USwitcherPauseWidget> WBPSwitcherPause;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGameOverWidget> WBPGameOver;

private:
	bool bIsMenuOpen = false;

	UFUNCTION()
	void HandleGameOverSwitch(float Percent);
	UFUNCTION()
	void HandlePauseInput();
	UFUNCTION()
	void ClosePauseMenu();
};
