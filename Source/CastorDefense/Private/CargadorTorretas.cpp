// Fill out your copyright notice in the Description page of Project Settings.


#include "CargadorTorretas.h"

// Sets default values for this component's properties
UCargadorTorretas::UCargadorTorretas()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int UCargadorTorretas::GetCapacidadMax()
{
	return this->CapacidadMax;
}

void UCargadorTorretas::SetCapacidadMax(int InCapacidadMaxima)
{
	this->CapacidadMax = InCapacidadMaxima;
}

int UCargadorTorretas::GetMunicion()
{
	return this->Municion;
}

void UCargadorTorretas::SetMunicion(int InMunicion)
{
	this->Municion = InMunicion;
}

TArray<ABala*> UCargadorTorretas::GetBalas()
{
	return this->Balas;
}

void UCargadorTorretas::SetBalas(TArray<ABala*> InBalas)
{
	this->Balas = InBalas;
}

int UCargadorTorretas::GetIndexBalas()
{
	return this->IndexBalas;
}

void UCargadorTorretas::SetIndexBalas(int InIndexBalas)
{
	this->IndexBalas = InIndexBalas;
}

void UCargadorTorretas::Disparo()
{

}

void UCargadorTorretas::Recargar(/*FALTA TipoMunicion*/) {

}


// Called when the game starts
void UCargadorTorretas::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCargadorTorretas::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

