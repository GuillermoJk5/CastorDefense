// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

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
	UFUNCTION(Category = "Getter")
	UZonaManager* GetZonaManager();

	UFUNCTION(Category = "Setter")
	void SetZonaManager(UZonaManager* InZonaManager);

	UFUNCTION(Category = "Getter")
	TArray<AZona*> GetMapaZonas();

	UFUNCTION(Category = "Setter")
	void SetMapaZonas(TArray<AZona*> InMapaZonas);

	UFUNCTION(Category = "Getter")
	float GetTamanyoCasilla();

	UFUNCTION(Category = "Setter")
	void SetTamanyoCasilla(float InTamanyoCasilla);

	UFUNCTION(Category = "Getter")
	int GetTamanyoLadoZona();

	UFUNCTION(Category = "Setter")
	void SetTamanyoLadoZona(int InTamanyoLadoZona);

	UFUNCTION(Category = "Getter")
	float GetTamanyoLadoMundo();

	UFUNCTION(Category = "Setter")
	void SetTamanyoLadoMundo(int InTamanyoLadoMundo);

	UFUNCTION(Category = "Getter")
	struct FEstadosMundo GetEstados();

	UFUNCTION(Category = "Setter")
	void SetEstados(struct FEstadosMundo InEstados);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosZIndexCreados(TArray<int> InZIndexCreados);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosZIndexAdyacentes(TArray<int> InZIndexAdyacentes);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosZBuscarMas(bool InZBuscarMas);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosFin(bool InFin);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosPosicionesPuentes(TArray<FVector2D> InPosicionesPuentes);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosMapaAPintar(TArray<ETipoCasilla> InMapaAPintar);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosPosicionActual(FVector2D InPosicionActual);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosMovimientoDinamico(TArray<FVector2D> InMovimientoDinamico);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosCantidad(int InCantidad);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosZonaTerminada(bool InZonaTerminada);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosActually(uint8 InActually);

	UFUNCTION(Category = "SetterEstados")
	UMundoManager* SetEstadosTieneQueSerInterseccion(bool InTieneQueSerInterseccion);

	UFUNCTION(Category = "SetterEstados")
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

	UFUNCTION()
	void InstanciarCasillas();

	UFUNCTION()
	void GuardarInstancias(ETipoCasilla Tipo, int Cantidad);

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
