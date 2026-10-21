// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemigo/Loot/Loot.h"

// Sets default values
ALoot::ALoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ETipoDeLoot ALoot::GetTipoDeLoot(){
	return this->TipoLoot;
}

void  ALoot::SetTipoDeLoot(ETipoDeLoot InTipoDeLoot){
	this->TipoLoot = InTipoDeLoot;
}

uint8 ALoot::GetCantidad() {
	return this->Cantidad;
}

void ALoot::SetCantidad(uint8 InCantidad) {
	this->Cantidad = InCantidad;
}

// Called when the game starts or when spawned
void ALoot::BeginPlay()
{
	TipoLoot = ETipoDeLoot::Escamas;
	Cantidad = 0;
	Super::BeginPlay();
	
}

// Called every frame
void ALoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

