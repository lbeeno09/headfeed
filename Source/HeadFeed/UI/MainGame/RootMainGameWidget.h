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
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void TraversePauseTree();
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsGameHUD();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcherRoot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGameUIWidget> WBPGameUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USwitcherPauseWidget> WBPSwitcherPause;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGameOverWidget> WBPGameOver;

private:
	UFUNCTION()
	void HandleResume();
	UFUNCTION()
	void HandleGameOver(float Percent);
};
