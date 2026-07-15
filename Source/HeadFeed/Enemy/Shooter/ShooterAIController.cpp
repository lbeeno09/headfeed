// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shooter/ShooterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/HFPlayer.h"

AShooterAIController::AShooterAIController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AShooterAIController::OnPerceptionUpdated);
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	if(BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void AShooterAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(AHFPlayer* HFPlayer = Cast<AHFPlayer>(Actor))
	{
		if(UBlackboardComponent* BB = GetBlackboardComponent())
		{
			bool bCanSee = Stimulus.WasSuccessfullySensed();
			BB->SetValueAsBool(TEXT("CanSeePlayer"), bCanSee);

			if(bCanSee)
			{
				BB->SetValueAsObject(TEXT("TargetActor"), HFPlayer);
			}
		}
	}
}
