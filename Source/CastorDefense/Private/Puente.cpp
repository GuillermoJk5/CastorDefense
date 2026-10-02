// Fill out your copyright notice in the Description page of Project Settings.


#include "Puente.h"
#include "Zona.h"

// Sets default values
APuente::APuente()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APuente::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuente::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AZona* APuente::GetConectadoCon()
{
	return this->ConectadoCon;
}

void APuente::SetConectadoCon(AZona* InConectadoCon)
{
	this->ConectadoCon = InConectadoCon;
}

bool APuente::GetBajado()
{
	return this->Bajado;
}

void APuente::SetBajado(bool InBajado)
{
	this->Bajado = InBajado;
}

void APuente::BajarPuente()
{
	this->SetActorRelativeRotation(FRotator(0.f, 160.f, 0.f));
	this->SetBajado(true);
}

