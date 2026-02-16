// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HFUI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class HEADFEED_API UHFUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category="Head Feed", meta=(DisplayName="Update Score"))
	void BP_UpdateScore(int32 Score);
};
