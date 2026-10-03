// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TiposDeDatosComunes/Struct/FDatosCasillas.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemigoManager.generated.h"

class AEnemigo;
class UMundoManager;
class AZona;
class UZonaManager;

/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API UEnemigoManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(Category = "Getter")
	UMundoManager* GetMundoManager();

	UFUNCTION(Category = "Setter")
	void SetMundoManager(UMundoManager* InMundoManager);

	UFUNCTION(Category = "Getter")
	TArray<AEnemigo*> GetEnemigosGestionables();

	UFUNCTION(Category = "Setter")
	void SetEnemigosGestionables(TArray<AEnemigo*> InEnemigosGestionables);

	UFUNCTION(Category = "EventGraph")
	void ActivarEnemigo(AZona* Zona, FDatosCasillas CasillaSpawn);

	UFUNCTION()
	TArray<FDatosCasillas> CalcularCaminoASeguir(FVector2D CoordenadasOdjetivo, AZona* ZonaSpawn, FDatosCasillas DatosCasillaActual);
	
	UFUNCTION()
	TMap<FDatosCasillas, AZona*> ComprobarCaminosAdyacentes(AZona* Zona, FVector2D Posicion, TMap<FDatosCasillas, FDatosCasillas> CaminoFinal);

	UFUNCTION()
	void ComprobarAdyacentesYX(FVector2D PosicionCentral, float Valor, float ValorComprobacionContraria, AZona* Zona, bool YX, bool& Agregar, AZona*& Z, FVector2D& PosicionPosible );

	UFUNCTION()
	TArray<AEnemigo*> GenerarEnemigosInicioPartida();

	UFUNCTION()
	void DesActivarEnemigo(AEnemigo* Enemigo, bool Activar);

	UFUNCTION()
	void PosicionarEnemigo(TArray<FDatosCasillas> Ruta, FDatosCasillas DatosCasilla, AZona* Zona);

private:

	UPROPERTY()
	UMundoManager* MundoManager;

	UPROPERTY()
	TArray<AEnemigo*> EnemigosGestionables;


};
