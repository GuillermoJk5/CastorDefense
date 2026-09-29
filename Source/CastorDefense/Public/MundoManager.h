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
struct FestadoMundo
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int> zIndexCreados;

	UPROPERTY()
	TArray<int> zIndexAdyacentes;

	UPROPERTY()
	bool zBuscarMas;

	UPROPERTY()
	bool fin;

	UPROPERTY()
	TArray<FVector2D> PosicionesPuentes;

	UPROPERTY()
	TArray<ETipoCasilla> mapaAPintar;

	UPROPERTY()
	FVector2D posicionActual;

	UPROPERTY()
	TArray<FVector2D> movimientoDinamico;

	UPROPERTY()
	int cantidad;

	UPROPERTY()
	bool ZonaTerminada;

	UPROPERTY()
	uint8 actually;

	UPROPERTY()
	bool tieneQueSerInterseccion;

	UPROPERTY()
	int cantidadFinCiclos;


	FestadoMundo() 
		:zIndexCreados()
		,zIndexAdyacentes()
		,zBuscarMas(true)
		,fin(false)
		,PosicionesPuentes()
		//,mapaApintar()
		,posicionActual(0.f , 0.f)
		,movimientoDinamico()
		,cantidad(0)
		,ZonaTerminada(true)
		,actually(0)
		,tieneQueSerInterseccion(false)
		,cantidadFinCiclos(0)
	{}

};
/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API UMundoManager : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:

	
	UPROPERTY()
	TArray<AZona*> mapaZonas;
	

	UPROPERTY()
	UZonaManager* zonaManager;

	UPROPERTY()
	float tamanyoCasilla;

	UPROPERTY()
	int TamanyoLadoZona;

	UPROPERTY()
	int tamanyoLadoMundo;

	UPROPERTY()
	struct FestadoMundo Estados;

	/*
	UPROPERTY(EditAnywhere)
	int meshCasilla
	
	UPROPERTY()
	int enemigosManager;

	UPROPERTY()
	int actorObjetivo;

	UPROPERTY()
	int gestorOleadas;
	*/


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:

	void AsignarObjetos();

	FTimerHandle TimerHandle_AssignSubsystem;

};
