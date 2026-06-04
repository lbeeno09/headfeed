// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HFEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class HEADFEED_API AHFEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AHFEnemyController();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, struct FAIStimulus Stimulus);
	
	// Common
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|AI")
	class UAIPerceptionComponent* PerceptionComp;
	
	// Assign @ Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Head Feed|AI")
	class UBehaviorTree* BTAsset;

	class UAISenseConfig_Sight* SightConfig;
};
