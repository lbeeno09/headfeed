// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/HFShooterAIController.h"
#include "AI/HFShooter.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"

AHFShooterAIController::AHFShooterAIController()
{
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("State Tree AI"));

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AHFShooterAIController::OnPerceptionUpdated);

	bSetControlRotationFromPawnOrientation = false;
}

void AHFShooterAIController::BeginPlay()
{
	TargetEnemy = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AHFShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(AHFShooter* Shooter = Cast<AHFShooter>(InPawn))
	{
		Shooter->OnPawnDeath.AddDynamic(this, &AHFShooterAIController::OnPawnDeath);
	}

	if(StateTreeAI)
	{
		StateTreeAI->StartLogic();
	}
}

void AHFShooterAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);

	if(TargetEnemy && bUpdatePawn)
	{
		FVector FocalPoint = TargetEnemy->GetActorLocation();
		FVector Direction = FocalPoint - GetPawn()->GetActorLocation();
		FRotator TargetRot = Direction.Rotation();

		TargetRot.Pitch = 0.0f;
		TargetRot.Roll = 0.0f;

		SetControlRotation(TargetRot);
	}
}

void AHFShooterAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		TargetEnemy = Actor;
	}
	else if(TargetEnemy == Actor)
	{
		TargetEnemy = nullptr;
	}
}

void AHFShooterAIController::OnPawnDeath()
{
	if(GetPathFollowingComponent())
	{
		GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);
	}

	if(StateTreeAI)
	{
		StateTreeAI->StopLogic(FString("Pawn Died"));
	}

	UnPossess();
}
