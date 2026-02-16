// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HFWeaponPickup.h"
#include "Weapons/HFWeaponHolder.h"
#include "Weapons/HFWeapon.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AHFWeaponPickup::AHFWeaponPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 84.0f));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldStatic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->bFillCollisionUnderneathForNavmesh = true;
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AHFWeaponPickup::OnOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionProfileName(FName("NoCollision"));
}

void AHFWeaponPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(FWeaponTableRow* WeaponData = WeaponType.GetRow<FWeaponTableRow>(FString()))
	{
		Mesh->SetStaticMesh(WeaponData->StaticMesh.LoadSynchronous());
	}
}

void AHFWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
	if(FWeaponTableRow* WeaponData = WeaponType.GetRow<FWeaponTableRow>(FString()))
	{
		WeaponClass = WeaponData->WeaponToSpawn;
	}
}

void AHFWeaponPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AHFWeaponPickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IHFWeaponHolder* WeaponHolder = Cast<IHFWeaponHolder>(OtherActor))
	{
		WeaponHolder->AddWeaponClass(WeaponClass);

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AHFWeaponPickup::RespawnPickup, RespawnTime, false);
	}
}

void AHFWeaponPickup::RespawnPickup()
{
	SetActorHiddenInGame(false);

	BP_OnRespawn();
}

void AHFWeaponPickup::FinishRespawn()
{
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}