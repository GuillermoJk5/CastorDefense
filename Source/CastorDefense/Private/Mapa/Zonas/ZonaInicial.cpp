// Fill out your copyright notice in the Description page of Project Settings.


#include "Mapa/Zonas/ZonaInicial.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"


AZonaInicial::AZonaInicial()
{
	HI_Objetivo = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HI_Objetivo"));
	HI_Objetivo->SetupAttachment(Root);

}

void AZonaInicial::BeginPlay()
{
	Super::BeginPlay();
	HI.Add(ETipoCasilla::Objetivo, HI_Objetivo);

}
