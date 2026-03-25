// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HFWeapon.h"
#include "Components/StaticMeshComponent.h"

AHFWeapon::AHFWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	CurrentAmmo = MaxAmmo;
}

void AHFWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateAmmoUI();
}

void AHFWeapon::PrimaryFire()
{
	if(CurrentAmmo > 0)
	{
		CurrentAmmo--;
		UpdateAmmoUI();
	}
	else
	{
		Reload();
	}
}

void AHFWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;
	UpdateAmmoUI();
}

void AHFWeapon::UpdateAmmoUI()
{
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
}
