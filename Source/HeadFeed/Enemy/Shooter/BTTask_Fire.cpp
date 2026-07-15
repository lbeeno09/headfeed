// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Shooter/BTTask_Fire.h"
#include "AIController.h"
#include "Enemy/Shooter/ShooterEnemy.h"
#include "Components/WeaponComponent.h"

UBTTask_Fire::UBTTask_Fire()
{
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Fire::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
