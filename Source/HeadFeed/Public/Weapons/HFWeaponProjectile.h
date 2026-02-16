// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HFWeaponProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class ACharacter;
class UPrimitiveComponent;

UCLASS(Abstract)
class HEADFEED_API AHFWeaponProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
protected:
	UPROPERTY(EditAnywhere, Category="Projectile|Noise", meta=(ClampMin=0, ClampMax=100))
	float NoiseLoudness = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile|Noise", meta = (ClampMin = 0, ClampMax = 100000, Units="cm"))
	float NoiseRange = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	FName NoiseTag = FName("Projectile");

	UPROPERTY(EditAnywhere, Category = "Projectile|Hit", meta = (ClampMin = 0, ClampMax = 50000))
	float PhysicsForce = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile|Hit", meta = (ClampMin = 0, ClampMax = 100))
	float HitDamage = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile|Hit")
	TSubclassOf<UDamageType> HitDamageType;

	UPROPERTY(EditAnywhere, Category = "Projectile|Hit")
	bool bDamageOwner = false;

	UPROPERTY(EditAnywhere, Category = "Projectile|Explosion")
	bool bExplodeOnHit = false;

	UPROPERTY(EditAnywhere, Category = "Projectile|Explosion", meta=(ClampMin=0, ClampMax=5000, Units="cm"))
	float ExplosionRadius = 500.0f;

	bool bHit = false;

	UPROPERTY(EditAnywhere, Category = "Projectile|Destruction", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float DeferredDestructionTime = 5.0f;

	FTimerHandle DestructionTimer;

public:	
	AHFWeaponProjectile();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:	
	void ExplosionCheck(const FVector& ExplosionCenter);

	void ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection);

	UFUNCTION(BlueprintImplementableEvent, Category="Projectile", meta=(DisplayName="On Projectile Hit"))
	void BP_OnProjectileHit(const FHitResult& Hit);

	void OnDeferredDestruction();
};
