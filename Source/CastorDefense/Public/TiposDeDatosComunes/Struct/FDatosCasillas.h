// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Puente.h"
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"
#include "FDatosCasillas.generated.h"

USTRUCT(BlueprintType)
struct FDatosCasillas
{
	GENERATED_BODY()

	UPROPERTY()
	int IndexLocal;

	UPROPERTY()
	ETipoCasilla Tipo;

	UPROPERTY()
	FVector2D GridPosition;

	UPROPERTY()
	bool Torreta;

	UPROPERTY()
	APuente* PuenteAsociado;

	FDatosCasillas()
		:IndexLocal()
		, Tipo()
		, GridPosition(0.f, 0.f)
		, Torreta()
		, PuenteAsociado()
	{
	}

	FDatosCasillas(int Index, ETipoCasilla Tipo, FVector2D GridPosition,bool Torreta, APuente* PuenteAsociado)
		:IndexLocal(Index)
		, Tipo(Tipo)
		, GridPosition(GridPosition)
		, Torreta(Torreta)
		, PuenteAsociado(PuenteAsociado)
	{
	}

};