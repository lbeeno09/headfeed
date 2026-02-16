// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HFWeaponHolder.h"
#include "Animation/AnimInstance.h"
#include "HFWeapon.generated.h"

class IHFWeaponHolder;
class AHFWeaponProjectile;
class USkeletalMeshComponent;
class UAnimMontage;
class UAnimInstance;

UCLASS(Abstract)
class HEADFEED_API AHFWeapon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ThirdPersonMesh;

protected:
	IHFWeaponHolder* WeaponOwner;

	UPROPERTY(EditAnywhere, Category="Ammo")
	TSubclassOf<AHFWeaponProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Ammo", meta=(ClampMin=0, ClampMax=100))
	int32 MagazineSize = 10;

	int32 CurrentBullets = 0;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* FiringMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;

	UPROPERTY(EditAnywhere, Category = "Aim", meta=(ClampMin=0, ClampMax=90, Units="Degrees"))
	float AimVariance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Aim", meta = (ClampMin = 0, ClampMax = 100))
	float FiringRecoil = 0.0f;

	UPROPERTY(EditAnywhere, Category="Aim")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = "Aim", meta = (ClampMin = 0, ClampMax = 1000, Units="cm"))
	float MuzzleOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Refire")
	bool bFullAuto = false;

	UPROPERTY(EditAnywhere, Category = "Refire", meta=(ClampMin=0, ClampMax=5, Units="s"))
	float RefireRate = 0.5f;

	float TimeOfLastShot = 0.0f;

	bool bIsFiring = false;

	FTimerHandle RefireTimer;

	TObjectPtr<APawn> PawnOwner;

	UPROPERTY(EditAnywhere, Category="Perception", meta=(ClampMin=0, ClampMax=100))
	float ShotLoudness = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Perception", meta = (ClampMin = 0, ClampMax = 100000, Units="cm"))
	float ShotNoiseRange = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Perception")
	FName ShotNoiseTag = FName("Shot");

public:	
	AHFWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

protected:
	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);

public:	
	void ActivateWeapon();

	void DeactivateWeapon();

	void StartFiring();

	void StopFiring();

protected:
	virtual void Fire();

	void FireCooldownExpired();

	virtual void FireProjectile(const FVector& TargetLocation);

	FTransform CalculateProjectileSpawnTransform(const FVector& TargetLocation) const;

public:
	UFUNCTION(BlueprintPure, Category="Weapon")
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; };

	UFUNCTION(BlueprintPure, Category = "Weapon")
	USkeletalMeshComponent* GetThirdPersonMesh() const { return ThirdPersonMesh; };

	const TSubclassOf<UAnimInstance>& GetFirstPersonAnimInstanceClass() const;

	const TSubclassOf<UAnimInstance>& GetThirdPersonAnimInstanceClass() const;

	int32 GetMagazineSize() const { return MagazineSize; };

	int32 GetBulletCount() const { return CurrentBullets; };
};
