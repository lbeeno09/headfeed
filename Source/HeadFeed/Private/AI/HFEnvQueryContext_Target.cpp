// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/HFEnvQueryContext_Target.h"
#include "AI/HFAIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UHFEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	if(AHFAIController* Controller = Cast<AHFAIController>(QueryInstance.Owner))
	{
		if(IsValid(Controller->GetCurrentTarget()))
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Controller->GetCurrentTarget());
		}
		else
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Controller);
		}
	}
}
