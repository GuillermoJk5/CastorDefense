// Fill out your copyright notice in the Description page of Project Settings.


#include "Mapa/Zonas/ZonaPortal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"


AZonaPortal::AZonaPortal()
{
	HI_SpawnEnemigo = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HI_SpawnEnemigo"));
	HI_SpawnEnemigo->SetupAttachment(Root);
}

void AZonaPortal::BeginPlay()
{
	Super::BeginPlay();
	HI.Add(ETipoCasilla::SpawnEnemigo, HI_SpawnEnemigo);
}
