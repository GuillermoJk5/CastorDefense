// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"
#include "FEstadosMundo.generated.h"

USTRUCT(BlueprintType)
struct FEstadosMundo
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


	FEstadosMundo()
		:ZIndexCreados()
		, ZIndexAdyacentes()
		, ZBuscarMas(true)
		, Fin(false)
		, PosicionesPuentes()
		, MapaAPintar()
		, PosicionActual(0.f, 0.f)
		, MovimientoDinamico()
		, Cantidad(0)
		, ZonaTerminada(true)
		, Actually(0)
		, TieneQueSerInterseccion(false)
		, CantidadFinCiclos(0)
	{}
};