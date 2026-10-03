// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Puente.h"
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"
#include "FDatosCasillas.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

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
	UHierarchicalInstancedStaticMeshComponent* HIDuenyo;

	UPROPERTY()
	bool Torreta;

	UPROPERTY()
	APuente* PuenteAsociado;

	bool operator==(const FDatosCasillas& Other) const
	{
		return IndexLocal == Other.IndexLocal 
			&& Tipo == Other.Tipo
			&& GridPosition == Other.GridPosition
			&& HIDuenyo == Other.HIDuenyo
			&& Torreta == Other.Torreta
			&& PuenteAsociado == Other.PuenteAsociado;
	}

	FDatosCasillas()
		:IndexLocal()
		, Tipo()
		, GridPosition(0.f, 0.f)
		, HIDuenyo()
		, Torreta()
		, PuenteAsociado()
	{
	}

	FDatosCasillas(ETipoCasilla Tipo, FVector2D GridPosition,bool Torreta, APuente* PuenteAsociado)
		:IndexLocal()
		, Tipo(Tipo)
		, GridPosition(GridPosition)
		, HIDuenyo()
		, Torreta(Torreta)
		, PuenteAsociado(PuenteAsociado)
	{
	}

	FDatosCasillas(int IndexLocal, ETipoCasilla Tipo, FVector2D GridPosition, UHierarchicalInstancedStaticMeshComponent* HIDuenyo, bool Torreta, APuente* PuenteAsociado)
		:IndexLocal(IndexLocal)
		, Tipo(Tipo)
		, GridPosition(GridPosition)
		, HIDuenyo(HIDuenyo)
		, Torreta(Torreta)
		, PuenteAsociado(PuenteAsociado)
	{
	}

};

FORCEINLINE uint32 GetTypeHash(const FDatosCasillas& Key)
{
	uint32 Hash = 0;

	Hash = HashCombine(Hash, GetTypeHash(Key.IndexLocal));
	Hash = HashCombine(Hash, GetTypeHash(Key.Tipo));
	Hash = HashCombine(Hash, GetTypeHash(Key.GridPosition));
	Hash = HashCombine(Hash, GetTypeHash(Key.HIDuenyo));
	Hash = HashCombine(Hash, GetTypeHash(Key.Torreta));
	Hash = HashCombine(Hash, GetTypeHash(Key.PuenteAsociado));
	return Hash;
}
