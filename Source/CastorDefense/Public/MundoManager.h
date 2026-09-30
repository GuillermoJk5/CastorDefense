// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MundoManager.generated.h"

class UZonaManager;
class AZona;

UENUM(BlueprintType)
enum class ETipoCasilla : uint8
{
	  NaN  UMETA(DisplayName = "NaN")
	, Terreno UMETA(DisplayName = "Terreno")
	, Camino  UMETA(DisplayName = "Camino")
	, CaminoConectado  UMETA(DisplayName = "CaminoConectado")
	, Puente  UMETA(DisplayName = "Puente")
	, PuenteConectado  UMETA(DisplayName = "PuenteConectado")
	, Objetivo  UMETA(DisplayName = "Objetivo")
	, SpawnEnemigo  UMETA(DisplayName = "SpawnEnemigo")
};

USTRUCT(BlueprintType)
struct FEstadoMundo
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int> ZIndexCreados;

	UPROPERTY()
	TArray<int> ZIndexAdyacentes;

	UPROPERTY()
	bool ZBuscarMas;

	UPROPERTY()
	bool Fin;

	UPROPERTY()
	TArray<FVector2D> PosicionesPuentes;

	UPROPERTY()
	TArray<ETipoCasilla> MapaAPintar;

	UPROPERTY()
	FVector2D PosicionActual;

	UPROPERTY()
	TArray<FVector2D> MovimientoDinamico;

	UPROPERTY()
	int Cantidad;

	UPROPERTY()
	bool ZonaTerminada;

	UPROPERTY()
	uint8 Actually;

	UPROPERTY()
	bool TieneQueSerInterseccion;

	UPROPERTY()
	int CantidadFinCiclos;


	FEstadoMundo() 
		:ZIndexCreados()
		,ZIndexAdyacentes()
		,ZBuscarMas(true)
		,Fin(false)
		,PosicionesPuentes()
		,MapaAPintar()
		,PosicionActual(0.f , 0.f)
		,MovimientoDinamico()
		,Cantidad(0)
		,ZonaTerminada(true)
		,Actually(0)
		,TieneQueSerInterseccion(false)
		,CantidadFinCiclos(0)
	{}

};

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
	struct FEstadoMundo GetEstados();

	UFUNCTION()
	void SetEstados(struct FEstadoMundo InEstados);

	UFUNCTION()
	void SetEstadosTieneQueSerInterseccion(bool InTieneQueSerInterseccion);

	UFUNCTION()
	void SetEstadosPosicionesPuentes(TArray<FVector2D> InPosicionesPuentes);

	UFUNCTION()
	void SetEstadosCantidadFinCiclos(int InCantidadFinCiclos);

	UFUNCTION()
	void SetEstadosMapaAPintar(TArray<ETipoCasilla> InMapaAPintar);

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
	struct FEstadoMundo Estados;

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
