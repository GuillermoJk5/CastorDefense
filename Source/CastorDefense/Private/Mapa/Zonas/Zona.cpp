// Fill out your copyright notice in the Description page of Project Settings.


#include "Mapa/Zonas/Zona.h"
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
AZona::AZona()
	:Interseccion(false)
	,DatosCasillas()
	,DatosCasillasPuentes()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	HI_Terreno = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HI_Terreno"));
	HI_Terreno->SetupAttachment(Root);
	HI_Camino = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HI_Camino"));
	HI_Camino->SetupAttachment(Root);
	HI_Puente = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HI_Puente"));
	HI_Puente->SetupAttachment(Root);
	
	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZona::BeginPlay()
{
	Super::BeginPlay();
	
	HI.Add(ETipoCasilla::Terreno, HI_Terreno);
	HI.Add(ETipoCasilla::Camino, HI_Camino);
	HI.Add(ETipoCasilla::Puente, HI_Puente);
}

// Called every frame
void AZona::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AZona::GetInterseccion()
{
	return this->Interseccion;
}

void AZona::SetInterseccion(bool InInterseccion)
{
	this->Interseccion = InInterseccion;
}

TArray<FDatosCasillas>& AZona::GetDatosCasillas()
{
	return this->DatosCasillas;
}

void AZona::SetDatosCasillas(TArray<FDatosCasillas> InDatosCasillas)
{
	this->DatosCasillas = InDatosCasillas;
}

TArray<FDatosCasillas>& AZona::GetDatosCasillasPuentes()
{
	return this->DatosCasillasPuentes;
}

void AZona::SetDatosCasillasPuentes(TArray<FDatosCasillas> InDatosCasillas)
{
	this->DatosCasillas = InDatosCasillas;
}

TMap<ETipoCasilla, UHierarchicalInstancedStaticMeshComponent*> AZona::GetHI()
{
	return this->HI;
}

void AZona::SetHI(TMap<ETipoCasilla, UHierarchicalInstancedStaticMeshComponent*> InHI)
{
	this->HI = InHI;
}
