// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeComponent.generated.h"

UENUM(BlueprintType)
enum class EMeleeType : uint8
{
	Club,
	Finish,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEADFEED_API UMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMeleeComponent();

	UFUNCTION(BlueprintCallable)
	void Attack();

	float GetMeleeRate() const { return MeleeRate; }

protected:
	virtual void BeginPlay() override;

	void ResetCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float MeleeDamage = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float MeleeRate = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float MaxRange = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	class UStaticMesh* MeleeHandMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	float SwingWidth = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	float SwingHeight = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	FRotator SwingRotation = FRotator(90.0f, 0.0f, 0.0f);

	FTimerHandle MaleeRateTimerHandle;
	bool bCanMelee = true;
};
