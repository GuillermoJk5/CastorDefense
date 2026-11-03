// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "OpcionesDeDesarrollo.generated.h"

class AZona;
class AZonaInicial;
class AZonaPortal;
class APuente;
class ANucleo;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Spawn Settings"))
class CASTORDEFENSE_API UOpcionesDeDesarrollo : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	const TArray<TSubclassOf<AZona>> GetClasesZonas() const { return this->ClasesZonas; };

	UFUNCTION()
	const TArray<TSubclassOf<AZonaInicial>> GetClasesZonasIniciales() const { return this->ClasesZonasIniciales; };

	UFUNCTION()
	const TArray<TSubclassOf<AZonaPortal>> GetClasesZonasPortales() const { return this->ClasesZonasPortales; };

	UFUNCTION()
	TSubclassOf<APuente> GetClasePuente() const { return this->ClasePuente; };

	UFUNCTION()
	TSubclassOf<ANucleo> GetClaseObjetivo() const { return this->ClaseObjetivo; };

private:

	UPROPERTY(Config, EditAnywhere, Category = "SpawnZona", meta = (AllowPrivateAcces = "true"))
	TArray<TSubclassOf<AZona>> ClasesZonas;

	UPROPERTY(Config, EditAnywhere, Category = "SpawnZonaInicial", meta = (AllowPrivateAcces = "true"))
	TArray<TSubclassOf<AZonaInicial>> ClasesZonasIniciales;

	UPROPERTY(Config, EditAnywhere, Category = "SpawnZonaPortal", meta = (AllowPrivateAcces = "true"))
	TArray<TSubclassOf<AZonaPortal>> ClasesZonasPortales;

	UPROPERTY(Config, EditAnywhere, Category = "SpawnPuente", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<APuente> ClasePuente;

	UPROPERTY(Config, EditAnywhere, Category = "SpawnObjetivo", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<ANucleo> ClaseObjetivo;
};
