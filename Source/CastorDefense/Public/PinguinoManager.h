// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PinguinoManager.generated.h"

class UZonaManager;
/**
 * Clase que se encarga de realizar el metodo del pingüino borracho para la generación de mapa
 */
UCLASS(Blueprintable)
class CASTORDEFENSE_API UPinguinoManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UZonaManager* zonaManager;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
 
    void AsignarObjetos();
 
    FTimerHandle TimerHandle_AssignSubsystem;
};
