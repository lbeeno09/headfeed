// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/HFEnemyController.h"
#include "Enemies/HFGunnerEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AHFEnemyController::AHFEnemyController()
{
	PrimaryActorTick.bCanEverTick = false;

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AHFEnemyController::BeginPlay()
{
	Super::BeginPlay();

	// Bind detection
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AHFEnemyController::OnTargetDetected);
	if(BTAsset)
	{
		RunBehaviorTree(BTAsset);
	}
}

void AHFEnemyController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		SetFocus(Actor);
		if(GetBlackboardComponent())
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
		}
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		if(GetBlackboardComponent())
		{
			GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		}
	}
}
