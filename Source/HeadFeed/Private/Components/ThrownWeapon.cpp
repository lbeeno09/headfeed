// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ThrownWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"

AThrownWeapon::AThrownWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(20.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 3500.0f;
	ProjectileMovement->MaxSpeed = 3500.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;
}

void AThrownWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->OnComponentHit.AddDynamic(this, &AThrownWeapon::OnProjectileHit);
}

void AThrownWeapon::InitializeThrow(UStaticMesh* WeaponMesh, float StunPower, float DamageValue)
{
	ThrowStun = StunPower;
	ThrowDamage = DamageValue;

	if(WeaponMesh)
	{
		MeshComponent->SetStaticMesh(WeaponMesh);
	}

	// TODO: Rotation
}

void AThrownWeapon::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(ThrowDamage, DamageEvent, GetInstigatorController(), this);
	}

	Destroy();
}
