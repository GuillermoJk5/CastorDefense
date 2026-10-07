// Fill out your copyright notice in the Description page of Project Settings.


#include "Torreta.h"
#include "Enemigo.h"
#include "TimerManager.h"
#include "CargadorTorretas.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEnemigo* ATorreta::GetObjetivo()
{
	return this->Objetivo;
}

void ATorreta::SetObjetivo(AEnemigo* InObjetivo)
{
	this->Objetivo = InObjetivo;
}

TArray<AEnemigo*> ATorreta::GetEnRango()
{
	return this->EnRango;
}

void ATorreta::SetEnRango(TArray<AEnemigo*> InEnRango)
{
	this->EnRango = InEnRango;
}

UCargadorTorretas* ATorreta::GetCargadorTorreta()
{
	return this->AC_CargadorTorreta;
}

void ATorreta::SetCargadorTorreta(UCargadorTorretas* InAC_CargadorTorreta)
{
	this->AC_CargadorTorreta = InAC_CargadorTorreta;
}

USphereComponent* ATorreta::GetAreaDeteccion()
{
	return this->AreaDeteccion;
}

void ATorreta::SetAreaDeteccion(USphereComponent* InAreaDeteccion)
{
	this->AreaDeteccion = InAreaDeteccion;
}

USphereComponent* ATorreta::GetAreaRango()
{
	return this->AreaRango;
}

void ATorreta::SetAreaRango(USphereComponent* InAreaRango)
{
	this->AreaRango = InAreaRango;
}

FTimerHandle ATorreta::GetTiempoProximoDisparo()
{
	return this->TiempoProximoDisparo;
}

void ATorreta::SetTiempoProximoDisparo(FTimerHandle InTiempoProximoDisparo)
{
	this->TiempoProximoDisparo = InTiempoProximoDisparo;
}

void ATorreta::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (this->Activada) {

		if (Cast<AEnemigo>(OtherActor)) {
			AEnemigo* Enemigo = Cast<AEnemigo>(OtherActor);
			EnRango.Add(Enemigo);
			if (!Objetivo) {
				Objetivo = Enemigo;
			}

			GetWorldTimerManager().SetTimer(
				TiempoProximoDisparo,
				this,
				&ATorreta::Disparar, //Valorar LLamar al metodo de disparo de el component directamente
				1.f,
				true
			);
		}
	}
}

void ATorreta::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (this->Activada) {
		if (Cast<AEnemigo>(OtherActor)) {
			AEnemigo* Enemigo = Cast<AEnemigo>(OtherActor);
			EnRango.Remove(Enemigo);
			if (EnRango[0]) {
				Objetivo = EnRango[0];
			}
			else {
				Objetivo = nullptr;
				GetWorldTimerManager().ClearTimer(TiempoProximoDisparo);
			}
		}
	}
}

void ATorreta::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!this->Activada) return;
	
	if (!Objetivo) return;

	SetActorRotation(FRotator(0.f, 0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Objetivo->GetActorLocation()).Yaw));
	
}

void ATorreta::ObjetivoSaldraDeArea()
{
	EnRango.Remove(Objetivo);

	if (EnRango[0]) {
		Objetivo = EnRango[0];
		Disparar();
	}
	else {
		Objetivo = nullptr; 
		GetWorldTimerManager().ClearTimer(TiempoProximoDisparo);
	}
}

void ATorreta::Recargar(/*FALTA Caja de municion*/)
{
	AC_CargadorTorreta->Recargar();
}

void ATorreta::Disparar()
{
	AC_CargadorTorreta->Disparo();
}


void ATorreta::BeginPlay()
{
	Super::BeginPlay();

	AC_CargadorTorreta = GetComponentByClass<UCargadorTorretas>();

	AreaDeteccion->OnComponentBeginOverlap.AddDynamic(this, &ATorreta::OnOverlapBegin);
	AreaDeteccion->OnComponentEndOverlap.AddDynamic(this, &ATorreta::OnOverlapEnd);
}

