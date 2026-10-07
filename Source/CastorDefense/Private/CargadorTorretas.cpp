// Fill out your copyright notice in the Description page of Project Settings.


#include "CargadorTorretas.h"
#include "Torreta.h"
#include "Enemigo.h"
#include "Bala.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	if (Municion > 0) {
		if (DisparoUtil()) {
			DispararBala();
		}
		else {
			ATorreta* Torreta = Cast<ATorreta>(GetOwner());
			Torreta->ObjetivoSaldraDeArea();
		}
	}
}

bool UCargadorTorretas::DisparoUtil()
{
	ATorreta* TorretaPerteneciente = Cast<ATorreta>(GetOwner());
	TArray<AActor*> PosiblesTorretasMundo;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATorreta::StaticClass(), PosiblesTorretasMundo);

	TArray<ATorreta*> RestoTorretasMundo;

	for (AActor* PosibleTorreta : PosiblesTorretasMundo) {
		if (Cast<ATorreta>(PosibleTorreta) && Cast<ATorreta>(PosibleTorreta) != TorretaPerteneciente) RestoTorretasMundo.Add(Cast<ATorreta>(PosibleTorreta));
	}

	if (RestoTorretasMundo.IsEmpty()) return true;
	
	float VidaEnemigo = TorretaPerteneciente->GetObjetivo()->GetVida();
	for (ATorreta* Torreta : RestoTorretasMundo) {
		if (!Torreta->GetActivada()) continue;
		if (Torreta->GetObjetivo() != TorretaPerteneciente->GetObjetivo()) continue;
		if (!Torreta->GetTiempoProximoDisparo().IsValid()) continue;
		if (!(GetWorld()->GetTimerManager().GetTimerRemaining(Torreta->GetTiempoProximoDisparo()) > GetWorld()->GetTimerManager().GetTimerRemaining(TorretaPerteneciente->GetTiempoProximoDisparo()))) continue;
		if (!(Torreta->GetCargadorTorreta()->GetMunicion() > 0)) continue;
		if (VidaEnemigo <= (Torreta->GetCargadorTorreta()->GetBalas()[Torreta->GetCargadorTorreta()->GetIndexBalas()]->GetDanyo())) {
			return false;
		}
		else {
			VidaEnemigo -= Torreta->GetCargadorTorreta()->GetBalas()[Torreta->GetCargadorTorreta()->GetIndexBalas()]->GetDanyo();
		}
	}

	return true;
}

void UCargadorTorretas::DispararBala()
{
	ATorreta* Torreta = Cast<ATorreta>(GetOwner());
	float VelocidadProyectil = 1000.f;
	float TiempoObjetivo = FVector::Dist(Torreta->GetActorLocation(), Torreta->GetObjetivo()->GetActorLocation()) / VelocidadProyectil;

	const float Tolerancia = 0.05f;
	FVector PosicionFinal;
	float Distancia;
	for (int Index = 0; Index < 20; Index++) {
		PosicionFinal = Torreta->GetObjetivo()->PredecirMovimiento(TiempoObjetivo);
		Distancia = FVector::Dist(Torreta->GetActorLocation(), PosicionFinal);

		if (FMath::Abs(Distancia / VelocidadProyectil - TiempoObjetivo) < Tolerancia) {
			break;
		}
		else {
			TiempoObjetivo = Distancia / VelocidadProyectil;
		}
	}
	if (Torreta->GetAreaRango()->GetScaledSphereRadius() > FVector::Dist(Torreta->GetAreaRango()->GetComponentLocation(), PosicionFinal)) {
		Balas[IndexBalas]->ActivarBala(Torreta->GetActorLocation(), UKismetMathLibrary::FindLookAtRotation(Torreta->GetActorLocation(), PosicionFinal));
		Municion--;
	}
	else {
		Torreta->ObjetivoSaldraDeArea();
	}
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

