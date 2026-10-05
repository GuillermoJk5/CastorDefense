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
	AZona* ZonaPerteneciente;

	bool operator == (const FMovimientoEnemigo& Item) const
	{
		return this->CantidadRecorrida == Item.CantidadRecorrida
			&& this->DistanciaAlObjetivo == Item.DistanciaAlObjetivo
			&& this->CosteMovimiento == Item.CosteMovimiento
			&& this->CasillaActual == Item.CasillaActual
			&& this->CasillaAnterior == Item.CasillaAnterior
			&& this->ZonaPerteneciente == Item.ZonaPerteneciente;
	}

	FMovimientoEnemigo()
		:CantidadRecorrida()
		, DistanciaAlObjetivo()
		, CosteMovimiento()
		, CasillaActual(FDatosCasillas())
		, CasillaAnterior(FDatosCasillas())
		, ZonaPerteneciente()
	{
	}

	FMovimientoEnemigo(int CantiadRecorrida, int DistanciaAlObjetivo , int CosteMovimiento , FDatosCasillas CasillaActual , FDatosCasillas CasillaAnterior, AZona* ZonaPerteneciente)
		:CantidadRecorrida(CantidadRecorrida)
		, DistanciaAlObjetivo(DistanciaAlObjetivo)
		, CosteMovimiento(CosteMovimiento)
		, CasillaActual(CasillaActual)
		, CasillaAnterior(CasillaAnterior)
		, ZonaPerteneciente(ZonaPerteneciente)
	{
	}
};
