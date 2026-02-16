// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeConditionBase.h"
#include "HFStateTreeUtility.generated.h"

class AHFNPC;
class AAIController;
class AHFAIController;

/*
 * Instance data struct for the FStateTreeLineOfSightToTargetCondition
 */
USTRUCT()
struct FStateTreeLineOfSightToTargetConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Context")
	AHFNPC* Character;

	UPROPERTY(EditAnywhere, Category = "Condition")
	AActor* Target;

	UPROPERTY(EditAnywhere, Category = "Condition")
	float LineOfSightConeAngle = 35.0f;

	UPROPERTY(EditAnywhere, Category = "Condition")
	int32 NumberOfVerticalLineOfSightChecks = 5;

	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bMustHaveLineOfSight = true;
};
STATETREE_POD_INSTANCEDATA(FStateTreeLineOfSightToTargetConditionInstanceData);

/*
 * StateTree condition to check if the character is grounded
 */
USTRUCT(DisplayName="Has Line of Sight to Target", Category="Shooter")
struct FStateTreeLineOfSightToTargetCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	// Set instance data type
	using FInstanceDataType = FStateTreeLineOfSightToTargetConditionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	FStateTreeLineOfSightToTargetCondition() = default;

	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};

////////////////////////////////////////////////////////////////////

/*
 * Instance data struct for the Face Towards Actor StateTree task
 */
USTRUCT()
struct FStateTreeFaceActorInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category ="Context")
	TObjectPtr<AAIController> Controller;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AActor> ActorToFaceTowards;
};

/*
 * StateTree task to face an AI-Controlled Pawn towards a Actor
 */
USTRUCT(meta=(DisplayName="Face Towards Actor", Category="Head Feed"))
struct FStateTreeFaceActorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	// Set instance data type
	using FInstanceDataType = FStateTreeFaceActorInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};

////////////////////////////////////////////////////////////////////

/*
 * Instance data struct for the Face Towards Location StateTree task
 */
USTRUCT()
struct FStateTreeFaceLocationInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AAIController> Controller;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FVector FaceLocation = FVector::ZeroVector;
};

/*
 * StateTree task to face an AI-Controlled Pawn towards a world location
 */
USTRUCT(meta = (DisplayName = "Face Towards Location", Category = "Head Feed"))
struct FStateTreeFaceLocationTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	// Set instance data type
	using FInstanceDataType = FStateTreeFaceLocationInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};

////////////////////////////////////////////////////////////////////

/*
 * Instance data struct for the Set Random Float StateTree task
 */
USTRUCT()
struct FStateTreeSetRandomFloatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MinValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MaxValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Output")
	float OutValue = 0.0f;
};

/*
 * StateTree task to calculate a random float value within the specified range
 */
USTRUCT(meta = (DisplayName = "Set Random Float", Category = "Head Feed"))
struct FStateTreeSetRandomFloatTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	// Set instance data type
	using FInstanceDataType = FStateTreeSetRandomFloatData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};

////////////////////////////////////////////////////////////////////

/*
 * Instance data struct for the Shoot at Target StateTree task
 */
USTRUCT()
struct FStateTreeShootAtTargetInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AHFNPC> Character;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AActor> Target;
};

/*
 * StateTree task to have an NPC shoot at an actor
 */
USTRUCT(meta = (DisplayName = "Shoot at Target", Category = "Head Feed"))
struct FStateTreeShootAtTargetTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	// Set instance data type
	using FInstanceDataType = FStateTreeShootAtTargetInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};

////////////////////////////////////////////////////////////////////

/*
 * Instance data struct for the Sense Enemies StateTree task
 */
USTRUCT()
struct FStateTreeSenseEnemiesInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AHFAIController> Controller;

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AHFNPC> Character;

	UPROPERTY(EditAnywhere, Category = "Output")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, Category = "Output")
	FVector InvestigateLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Output")
	bool bHasTarget = false;

	UPROPERTY(EditAnywhere, Category = "Output")
	bool bHasInvestigateLocation = false;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FName SenseTag = FName("Player");

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float DirectLineOfSightCone = 85.0f;

	UPROPERTY(EditAnywhere)
	float LastStimulusStrength = 0.0f;
};

/*
 * StateTree task to have an NPC process AI Perceptions and sense nearby enemies
 */
USTRUCT(meta = (DisplayName = "Sense Enemies", Category = "Head Feed"))
struct FStateTreeSenseEnemiesTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	// Set instance data type
	using FInstanceDataType = FStateTreeSenseEnemiesInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};