// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TiposDeDatosComunes/Struct/FDatosCasillas.h"
#include "FMovimientoEnemigo.generated.h"

class AZona;
 
USTRUCT(BlueprintType)
struct FMovimientoEnemigo{

	GENERATED_BODY()

	UPROPERTY()
	int CantidadRecorrida;
	
	UPROPERTY()
	int DistanciaAlObjetivo;

	UPROPERTY()
	int CosteMovimiento;

	UPROPERTY()
	FDatosCasillas CasillaActual;

	UPROPERTY()
	FDatosCasillas CasillaAnterior;

	UPROPERTY()
	AZona* Zona;

	FMovimientoEnemigo()
		:CantidadRecorrida()
		, DistanciaAlObjetivo()
		, CosteMovimiento()
		, CasillaActual(FDatosCasillas())
		, CasillaAnterior(FDatosCasillas())
		, Zona()
	{
	}
};
