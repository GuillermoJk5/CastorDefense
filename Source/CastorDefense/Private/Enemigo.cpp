// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemigo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemigoManager.h"


// Sets default values
AEnemigo::AEnemigo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemigo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemigo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemigo::GetVida()
{
	return this->Vida;
}

float AEnemigo::GetVidaMax()
{
	return  this->VidaMax;
}

float AEnemigo::GetDanyo()
{
	return this->Danyo;
}

int AEnemigo::GetIndex()
{
	return  this->Index;
}

TArray<FDatosCasillas> AEnemigo::GetRuta()
{
	return  this->Ruta;
}

void AEnemigo::SetVida(float InVida)
{
	this->Vida = InVida;
}

void AEnemigo::SetVidaMax(float InVidaMax)
{
	this->VidaMax = InVidaMax;
}

void AEnemigo::SetDanyo(float InDanyo)
{
	this->Danyo = InDanyo;
}

void AEnemigo::SetIndex(int InIndex)
{
	this->Index = InIndex;
}

void AEnemigo::SetRuta(TArray<FDatosCasillas> InRuta)
{
	this->Ruta = InRuta;
}

//FALTA AICONTROLLER
FVector AEnemigo :: PredecirMovimiento(float TiempoOdjetivo){

	FVector PosicionActual = this->GetActorLocation();

	//Cast<AIController>(this->GetOwner());

	return FVector();
}

void AEnemigo::RecibirHerida(float Herida)
{
	Vida = Vida - Herida;
	if (Vida <= 0) {
	
	//FALTA LOOT
	//SPAWN LOOT

	Destruir();
		
	}
}


void AEnemigo::Destruir()
{
	//FALTA DESACTIVARACTOR METODO GLOBAL
	//DesActivarEnemigo(this, false);

	this->SetActorLocation(
		FVector(0, 0, -10000),
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}

