// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/HFAIController.h"
#include "AI/HFNPC.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/Navigation/PathFollowingAgentInterface.h"

AHFAIController::AHFAIController()
{
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AHFAIController::OnPerceptionUpdated);
	AIPerception->OnTargetPerceptionForgotten.AddDynamic(this, &AHFAIController::OnPerceptionForgotten);
}

void AHFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(AHFNPC* NPC = Cast<AHFNPC>(InPawn))
	{
		NPC->Tags.Add(TeamTag);
		NPC->OnPawnDeath.AddDynamic(this, &AHFAIController::OnPawnDeath);
	}
}

void AHFAIController::OnPawnDeath()
{
	GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);

	StateTreeAI->StopLogic(FString(""));

	UnPossess();

	Destroy();
}

void AHFAIController::SetCurrentTarget(AActor* Target)
{
	TargetEnemy = Target;
}

void AHFAIController::ClearCurrentTarget()
{
	TargetEnemy = nullptr;
}

void AHFAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	OnShooterPerceptionUpdated.ExecuteIfBound(Actor, Stimulus);
}

void AHFAIController::OnPerceptionForgotten(AActor* Actor)
{
	OnShooterPerceptionForgotten.ExecuteIfBound(Actor);
}
