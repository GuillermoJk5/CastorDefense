// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

//Enum TipoCasillas
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"	

//Struck
#include "TiposDeDatosComunes/Struct/FEstadosMundo.h"
#include "MundoManager.generated.h"

class UZonaManager;
class AZona;

/**
 * 
 */
UCLASS(Blueprintable)
class CASTORDEFENSE_API UMundoManager : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;


	//Getters y Setters
	UFUNCTION()
	UZonaManager* GetZonaManager();

	UFUNCTION()
	void SetZonaManager(UZonaManager* InZonaManager);

	UFUNCTION()
	TArray<AZona*> GetMapaZonas();

	UFUNCTION()
	void SetMapaZonas(TArray<AZona*> InMapaZonas);

	UFUNCTION()
	float GetTamanyoCasilla();

	UFUNCTION()
	void SetTamanyoCasilla(float InTamanyoCasilla);

	UFUNCTION()
	int GetTamanyoLadoZona();

	UFUNCTION()
	void SetTamanyoLadoZona(int InTamanyoLadoZona);

	UFUNCTION()
	float GetTamanyoLadoMundo();

	UFUNCTION()
	void SetTamanyoLadoMundo(int InTamanyoLadoMundo);

	UFUNCTION()
	struct FEstadosMundo GetEstados();

	UFUNCTION()
	void SetEstados(struct FEstadosMundo InEstados);

	UFUNCTION()
	UMundoManager* SetEstadosZIndexCreados(TArray<int> InZIndexCreados);

	UFUNCTION()
	UMundoManager* SetEstadosZIndexAdyacentes(TArray<int> InZIndexAdyacentes);

	UFUNCTION()
	UMundoManager* SetEstadosZBuscarMas(bool InZBuscarMas);

	UFUNCTION()
	UMundoManager* SetEstadosFin(bool InFin);

	UFUNCTION()
	UMundoManager* SetEstadosPosicionesPuentes(TArray<FVector2D> InPosicionesPuentes);

	UFUNCTION()
	UMundoManager* SetEstadosMapaAPintar(TArray<ETipoCasilla> InMapaAPintar);

	UFUNCTION()
	UMundoManager* SetEstadosPosicionActual(FVector2D InPosicionActual);

	UFUNCTION()
	UMundoManager* SetEstadosMovimientoDinamico(TArray<FVector2D> InMovimientoDinamico);

	UFUNCTION()
	UMundoManager* SetEstadosCantidad(int InCantidad);

	UFUNCTION()
	UMundoManager* SetEstadosZonaTerminada(bool InZonaTerminada);

	UFUNCTION()
	UMundoManager* SetEstadosActually(uint8 InActually);

	UFUNCTION()
	UMundoManager* SetEstadosTieneQueSerInterseccion(bool InTieneQueSerInterseccion);

	UFUNCTION()
	UMundoManager* SetEstadosCantidadFinCiclos(int InCantidadFinCiclos);

	

	//Metodos

	UFUNCTION()
	void CrearNavMesh();

	UFUNCTION()
	AZona* GenerarEsquinaMundo(int index);

	UFUNCTION()
	void GenerarZonasDesdeInicio();

	UFUNCTION()
	void ComprobarZonasAdyacentes(FVector2D Coordenada, TArray<AZona*>& ZonasAdyacentes, TArray<int>& ZonasVacias);

	UFUNCTION()
	TArray<FVector2D> ObtenerPosiblesAdyacentes(FVector2D PosicionCentral, int TamanyoLadoMatriz);

	UFUNCTION()
	bool ComprobarLimitesDeMatriz(FVector2D Vector, int TamanyoLadoMatriz);
	
	UFUNCTION()
	TArray<FVector2D> ObtenerPosiblesDiagonalesAdyacentes(FVector2D PosicionCentral, int TamanyoLadoMatriz);

	FORCEINLINE FVector2D ObtenerCoordenadasDeUnaZonaConIndex(int index) {

		return FVector2D
		(
			(TamanyoCasilla * TamanyoLadoZona + TamanyoCasilla * 2) * (index % TamanyoLadoMundo)
			,(TamanyoCasilla * TamanyoLadoZona + TamanyoCasilla * 2) * (index / TamanyoLadoMundo)
		);

	}

private:

	UPROPERTY()
	TArray<AZona*> MapaZonas;

	UPROPERTY()
	UZonaManager* ZonaManager;

	UPROPERTY()
	float TamanyoCasilla;

	UPROPERTY()
	int TamanyoLadoZona;

	UPROPERTY()
	int TamanyoLadoMundo;

	UPROPERTY()
	struct FEstadosMundo Estados;

	/*
	UPROPERTY(EditAnywhere)
	int MeshCasilla
	
	UPROPERTY()
	int EnemigosManager;

	UPROPERTY()
	int ActorObjetivo;

	UPROPERTY()
	int GestorOleadas;
	*/

};
