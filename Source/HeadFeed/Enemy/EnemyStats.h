// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Active,
	//Stunned, // TODO: after skeleton made; allow melee to do finish move
	Dead
};

/**
 * 
 */
class HEADFEED_API EnemyStats
{
public:
	EnemyStats();
	~EnemyStats();
};
