// Fill out your copyright notice in the Description page of Project Settings.


#include "Mapa/Nucleo.h"
#include "Enemigo/Enemigo.h"
#include "ClaseDeUtilidades.h"

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

TArray<ACajaMunicion*> ANucleo::GetMunicionCreada()
{
	return this->MunicionCreada;
}

void ANucleo::SetMunicionCreada(TArray<ACajaMunicion*> inMunicionCreada)
{
	this->MunicionCreada = inMunicionCreada;
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

bool ANucleo::ActualizarEnergia(float CosteDeEnergia)
{
	EnergiaRestante = (EnergiaRestante + CosteDeEnergia) > EnergiaMax ? EnergiaMax : (EnergiaRestante + CosteDeEnergia);

	return EnergiaRestante >= 0;
}

void ANucleo::GenerarMunicion(ETipoMunicion TipoMunicion, ETamanyoMunicion TamanyoMunicion, AActor* Generador)
{
	//Si ya tenemos una CajaMunicion Inactiva usamos esa , sino creamos una nueva
	if (MunicionCreada.IsEmpty()) {

		ACajaMunicion* CajaMunicion = GetWorld()->SpawnActorDeferred<ACajaMunicion>(
			ACajaMunicion::StaticClass(),
			FTransform(),
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		if (CajaMunicion) {

			CajaMunicion->SetTipoMunicion(TipoMunicion);
			CajaMunicion->SetTamanyoMunicion(TamanyoMunicion);
			//Falta
			//CajaMunicion->SetIcono(Icono)

			CajaMunicion->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(Generador->GetActorLocation() + Generador->GetActorForwardVector() * 200.0f)));
		}
		else {

			MunicionCreada[0]->SetTipoMunicion(TipoMunicion);
			MunicionCreada[0]->SetTamanyoMunicion(TamanyoMunicion);
			//	MunicionCreada[0].SetIcono(Icono);

			MunicionCreada[0]->SetActorLocation(FVector(Generador->GetActorLocation() + Generador->GetActorForwardVector() * 200.0f));
			UClaseDeUtilidades::ActivarDesactivarActor(MunicionCreada[0], true);

			MunicionCreada.Remove(MunicionCreada[0]);

		}

	}



}

