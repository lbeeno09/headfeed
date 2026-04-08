// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HFWeapon.h"
#include "Components/StaticMeshComponent.h"

AHFWeapon::AHFWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AHFWeapon::PrimaryFire()
{
}

void AHFWeapon::Reload()
{
}
