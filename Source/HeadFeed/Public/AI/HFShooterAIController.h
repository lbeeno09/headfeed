// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "HFShooterAIController.generated.h"

class UStateTreeAIComponent;
class UAIPerceptionComponent;
//struct FAIStimulus;

/**
 * 
 */
UCLASS()
class HEADFEED_API AHFShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHFShooterAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|AI")
	TObjectPtr<AActor> TargetEnemy;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

	UFUNCTION()
	void OnPawnDeath();

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components", meta = (AllowPrivateAccess = "true"))
	UStateTreeAIComponent* StateTreeAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Head Feed|Components", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerception;

};
