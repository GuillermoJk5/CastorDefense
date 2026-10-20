// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TiposDeDatosComunes/Struct/FDatosCasillas.h"
#include "OleadasManager.generated.h"

class UMundoManager;
class UEnemigoManager;
class AZona;

/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API UOleadasManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Getter")
	UMundoManager* GetMundoManager();

	UFUNCTION(Category = "Setter")
	void SetMundoManager(UMundoManager* InMundoManager);

	UFUNCTION(Category = "Getter")
	UEnemigoManager* GetEnemigoManager();

	UFUNCTION(Category = "Setter")
	void SetEnemigoManager(UEnemigoManager* InEnemigoManager);

	UFUNCTION(Category = "Getter")
	int GetSaldoGeneracionEnemigo();

	UFUNCTION(Category = "Setter")
	void SetSaldoGeneracionEnemigo(int InSaldoGeneracionEnemigo);

	UFUNCTION(Category = "Getter")
	uint8 GetCantidadSpawns();

	UFUNCTION(Category = "Setter")
	void SetCantidadSpawns(uint8 InCantidadSpawns);

	UFUNCTION()
	void GenerarOleada();

	UFUNCTION()
	void JGO();

	UFUNCTION()
	void EncontrarSpawns(TArray<FDatosCasillas>& PosicionesSpawns, TArray<AZona*>& ZonasConSpawns);

	UFUNCTION()
	void GenerarEnemigosMientrasHayaSaldo(TArray<FDatosCasillas> PosicionesSpawns, TArray<AZona*> ZonasConSpawns);
	
private:
	
	UPROPERTY()
	UMundoManager* MundoManager;

	UPROPERTY()
	UEnemigoManager* EnemigoManager;

	UPROPERTY()
	int SaldoGeneracionEnemigo = 20;

	UPROPERTY()
	uint8 CantidadSpawns;

	FTimerHandle TimerHandle_GeneradorDeEnemigos;
};
