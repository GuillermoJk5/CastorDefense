// Fill out your copyright notice in the Description page of Project Settings.


#include "Nucleo.h"
#include "Enemigo.h"

// Sets default values
ANucleo::ANucleo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float ANucleo::GetVida()
{
	return this->Vida;
}

void ANucleo::SetVida(float InVida)
{
	this->Vida = InVida;
}

float ANucleo::GetEnergiaMax()
{
	return this->EnergiaMax;
}

void ANucleo::SetEnergiaMax(float InEnergiaMax)
{
	this->EnergiaMax = InEnergiaMax;
}

float ANucleo::GetEnergiaRestante()
{
	return this->EnergiaRestante;
}

void ANucleo::SetEnergiaRestante(float InEnergiaRestante)
{
	this->Vida = InEnergiaRestante;
}

// Called when the game starts or when spawned
void ANucleo::BeginPlay()
{
	Super::BeginPlay();
	this->SetEnergiaRestante(this->EnergiaMax);
	
}

// Called every frame
void ANucleo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANucleo::OnHandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	AEnemigo* Enemigo = Cast<AEnemigo>(OtherActor);
	if(Enemigo)
	{
		PerderVida(Enemigo->GetDanyo());
		Enemigo->Destruir();

		if (Vida <= 0) {
		
			//PORHACER METODO PERDER
		}

	}
}

void ANucleo::PerderVida(float Danyo){

	this->SetVida(this->Vida - Danyo);

}

