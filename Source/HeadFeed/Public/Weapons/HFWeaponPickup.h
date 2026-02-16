// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "HFWeaponPickup.generated.h"

class USphereComponent;
class UPrimitiveComponent;
class AHFWeapon;

USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHFWeapon> WeaponToSpawn;
};

UCLASS(Abstract)
class HEADFEED_API AHFWeaponPickup : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

protected:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	FDataTableRowHandle WeaponType;

	TSubclassOf<AHFWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta=(ClampMin=0, ClampMax=120, Units="s"))
	float RespawnTime = 4.0f;

	FTimerHandle RespawnTimer;

public:	
	AHFWeaponPickup();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	void RespawnPickup();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup", meta = (DisplayName = "On Respawn"))
	void BP_OnRespawn();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void FinishRespawn();
};
