// Fill out your copyright notice in the Description page of Project Settings.


#include "CajaMunicion.h"


// Sets default values
ACajaMunicion::ACajaMunicion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ETipoMunicion ACajaMunicion::GetTipoMunicion()
{
	return this->TipoMunicion;
}

void ACajaMunicion::SetTipoMunicion(ETipoMunicion InTipoMunicion)
{
	this->TipoMunicion = InTipoMunicion;
}

ETamanyoMunicion ACajaMunicion::GetTamanyoMunicion()
{
	return this->TamanyoMunicion;
}

void ACajaMunicion::SetTamanyoMunicion(ETamanyoMunicion InTamanyoMunicion)
{
	this->TamanyoMunicion = InTamanyoMunicion;
}

UTexture* ACajaMunicion::GetTextura()
{
	return this->Textura;
}

void ACajaMunicion::SetTextura(UTexture* InTextura)
{
	this->Textura = InTextura;
}



// Called when the game starts or when spawned
void ACajaMunicion::BeginPlay()
{
	Super::BeginPlay();
	//FALTA  emparentar, ya que la funcionalidad es mas del blueprint que de c++ - GenerarTipoMunicion();
}

// Called every frame
void ACajaMunicion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

