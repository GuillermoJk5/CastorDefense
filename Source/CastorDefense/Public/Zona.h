// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zona.generated.h"

class APuente;

UENUM(BlueprintType)
enum class ETipoCasillaAExportar : uint8
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
struct FDatosCasillas
{
	GENERATED_BODY()

	UPROPERTY()
	int IndexLocal;

	UPROPERTY()
	TArray<ETipoCasillaAExportar> Tipo;

	UPROPERTY()
	FVector2D GridPosition;

	UPROPERTY()
	bool zBuscarMas;

	UPROPERTY()
	bool Torreta;

	UPROPERTY()
	APuente* PuenteAsociado;

	FDatosCasillas()
		:IndexLocal()
		, Tipo()
		, GridPosition(0.f, 0.f)
		, zBuscarMas()
		, Torreta()
		,PuenteAsociado()
	{
	}

};

UCLASS()
class CASTORDEFENSE_API AZona : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZona();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Getter")
	bool GetInterseccion();

	UFUNCTION(Category = "Setter")
	void SetInterseccion(bool InInterseccion);

	UFUNCTION(Category = "Getter")
	FDatosCasillas GetDatosCasillas();

	UFUNCTION(Category = "Setter")
	void SetDatosCasillas(FDatosCasillas InInterseccion);

	UFUNCTION(Category = "Getter")
	FDatosCasillas GetDatosCasillasPuentes();

	UFUNCTION(Category = "Setter")
	void SetDatosCasillasPuentes(FDatosCasillas InInterseccion);
private:

	UPROPERTY(EditAnywhere)
	bool Interseccion;

	UPROPERTY(EditAnywhere)
	FDatosCasillas DatosCasillas;

	UPROPERTY(EditAnywhere)
	FDatosCasillas DatosCasillasPuentes;

};
