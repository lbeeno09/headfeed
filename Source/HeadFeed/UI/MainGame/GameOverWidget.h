// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverRetryClicked);

class UButton;

/**
 * 
 */
UCLASS()
class HEADFEED_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextResult;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonRetry;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonTitle;

private:
	UFUNCTION()
	void HandleResultText();
	UFUNCTION()
	void OnButtonRetryClicked();
	UFUNCTION()
	void OnButtonTitleClicked();
};
