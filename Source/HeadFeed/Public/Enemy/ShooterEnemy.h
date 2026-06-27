// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterEnemy.generated.h"

UCLASS()
class HEADFEED_API AShooterEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
