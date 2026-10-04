// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjetoColocable.h"

#include "Nucleo.h"

// Sets default values
AObjetoColocable::AObjetoColocable()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjetoColocable::BeginPlay()
{
	Super::BeginPlay();
	
	Activada = false;
}

// Called every frame
void AObjetoColocable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AObjetoColocable::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

ANucleo* AObjetoColocable::GetNucleo()
{
	return this->Nucleo;
}

void AObjetoColocable::SetNucleo(ANucleo* InNucleo)
{
	this->Nucleo = InNucleo;
}

float AObjetoColocable::GetCosteDeEnergia()
{
	return this->CosteDeEnergia;
}

void AObjetoColocable::SetCosteDeEnergia(float InCosteDEnergia)
{
	this->CosteDeEnergia = InCosteDEnergia;
}

bool AObjetoColocable::GetActivada()
{
	return this->Activada;
}

void AObjetoColocable::SetActivada(bool InActivada)
{
	this->Activada = InActivada;
}

ETipoCasilla AObjetoColocable::GetCasillaValida()
{
	return this->CasillaValida;
}

void AObjetoColocable::SetCasillaValida(ETipoCasilla InCasillaValida)
{
	this->CasillaValida = InCasillaValida;
}

void AObjetoColocable::DesActivar()
{
	Activada = !Activada;
}

void AObjetoColocable::CalcularEnergiaYActuvar()
{
	if (Nucleo->ActualizarEnergia(CosteDeEnergia *= -1)) {
		DesActivar();
	}
}

