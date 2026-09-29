// Fill out your copyright notice in the Description page of Project Settings.


#include "Zona.h"

// Sets default values
AZona::AZona()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZona::BeginPlay()
{
	Super::BeginPlay();
	
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

FDatosCasillas AZona::GetDatosCasillas()
{
	return this->DatosCasillas;
}

void AZona::SetDatosCasillas(FDatosCasillas InDatosCasillas)
{
	this->DatosCasillas = InDatosCasillas;
}

FDatosCasillas AZona::GetDatosCasillasPuentes()
{
	return this->DatosCasillasPuentes;
}

void AZona::SetDatosCasillasPuentes(FDatosCasillas InDatosCasillas)
{
	this->DatosCasillas = InDatosCasillas;
}

