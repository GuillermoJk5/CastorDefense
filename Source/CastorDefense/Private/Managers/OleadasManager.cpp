// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/OleadasManager.h"
#include "Mapa/Zonas/Zona.h"
#include "Mapa/Zonas/ZonaInicial.h"
#include "Mapa/Zonas/ZonaPortal.h"
#include "Managers/EnemigoManager.h"
#include "Managers/MundoManager.h"
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"

#include "TimerManager.h"

UMundoManager* UOleadasManager::GetMundoManager(){
	return this->MundoManager;
}

void UOleadasManager::SetMundoManager(UMundoManager* InMundoManager) {
	this->MundoManager = InMundoManager;
}

UEnemigoManager* UOleadasManager::GetEnemigoManager() {
	return this->EnemigoManager;
}

void UOleadasManager::SetEnemigoManager(UEnemigoManager* InEnemigoManager) {
	this->EnemigoManager = InEnemigoManager;
}

int UOleadasManager::GetSaldoGeneracionEnemigo() {
	return this->SaldoGeneracionEnemigo;
}

void UOleadasManager::SetSaldoGeneracionEnemigo(int InSaldoGeneracionEnemigo) {
	this->SaldoGeneracionEnemigo = InSaldoGeneracionEnemigo;
}

uint8 UOleadasManager::GetCantidadSpawns() {
	return this->CantidadSpawns;
}

void UOleadasManager::SetCantidadSpawns(uint8 InCantidadSpawns) {
	this->CantidadSpawns = InCantidadSpawns;
}

void UOleadasManager::GenerarOleada(){

}

void UOleadasManager::JGO(){
	SaldoGeneracionEnemigo = 20;
	TArray<FDatosCasillas> PosicionesSpawns;
	TArray<AZona*> ZonasConSpawns;
	EncontrarSpawns(PosicionesSpawns, ZonasConSpawns);
	GenerarEnemigosMientrasHayaSaldo(PosicionesSpawns, ZonasConSpawns);
}

void UOleadasManager::EncontrarSpawns(TArray<FDatosCasillas>& PosicionesSpawns, TArray<AZona*>& ZonasConSpawns){
	TArray<AZonaPortal*> ZonasPortal;
	TArray<AZona*> ZonasVisibles;
	for (AZona* Zona : MundoManager->GetMapaZonas()){
		if(!Zona->IsHidden()){
			if(AZonaPortal* Portal = Cast<AZonaPortal>(Zona)){
				ZonasPortal.AddUnique(Portal);
			}
			else if (!Zona->IsA<AZonaInicial>()) {
				ZonasVisibles.AddUnique(Zona);
			}
		}
	}

	for (AZonaPortal* Portal : ZonasPortal){
		if (Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1)/ 2) + 1].Tipo == ETipoCasilla::Camino){
			PosicionesSpawns.Add(Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1) / 2) + 1]);
			ZonasConSpawns.Add(Portal);
		}
		if (Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1) / 2) - 1].Tipo == ETipoCasilla::Camino) {
			PosicionesSpawns.Add(Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1) / 2) - 1]);
			ZonasConSpawns.Add(Portal);
		}
		if (Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1) / 2) + MundoManager->GetTamanyoLadoZona()].Tipo == ETipoCasilla::Camino) {
			PosicionesSpawns.Add(Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1) / 2) + MundoManager->GetTamanyoLadoZona()]);
			ZonasConSpawns.Add(Portal);
		}
		if (Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1) / 2) - MundoManager->GetTamanyoLadoZona()].Tipo == ETipoCasilla::Camino) {
			PosicionesSpawns.Add(Portal->GetDatosCasillas()[((Portal->GetDatosCasillas().Num() - 1) / 2) - MundoManager->GetTamanyoLadoZona()]);
			ZonasConSpawns.Add(Portal);
		}
	}

	for (AZona* Zona : ZonasVisibles){
		for(FDatosCasillas DatosPuente : Zona->GetDatosCasillasPuentes()){
			if(!DatosPuente.PuenteAsociado->GetBajado()){
				PosicionesSpawns.Add(DatosPuente);
				ZonasConSpawns.Add(Zona);
			}
		}
	}
}


void UOleadasManager::GenerarEnemigosMientrasHayaSaldo(TArray<FDatosCasillas> PosicionesSpawns, TArray<AZona*> ZonasConSpawns){
	
	int Index = 0;
	for (FDatosCasillas DatosCasilla : PosicionesSpawns) {
		Index++;
		if (SaldoGeneracionEnemigo <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_GeneradorDeEnemigos);
			return;
		}
		EnemigoManager->ActivarEnemigo(ZonasConSpawns[Index], DatosCasilla);
		SaldoGeneracionEnemigo -= 5;
	}

	if (SaldoGeneracionEnemigo > 0) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_GeneradorDeEnemigos, [this, PosicionesSpawns, ZonasConSpawns]() {GenerarEnemigosMientrasHayaSaldo(PosicionesSpawns, ZonasConSpawns);},3.f,false);
	}
	else {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_GeneradorDeEnemigos);
	}
	
}
