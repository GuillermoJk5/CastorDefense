// Fill out your copyright notice in the Description page of Project Settings.


#include "ZonaManager.h"
#include "PinguinoManager.h"
#include "Engine/Engine.h" 

void UZonaManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            TimerHandle_AssignSubsystem,
            this,
            &UZonaManager::AsignarObjetos,
            0.2f,
            true
        );
    }
}

void UZonaManager::AsignarObjetos()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UPinguinoManager* FoundSubsystem = World->GetSubsystem<UPinguinoManager>();

    if (FoundSubsystem)
    {
        pinguinoManager = FoundSubsystem;
        World->GetTimerManager().ClearTimer(TimerHandle_AssignSubsystem);

        UE_LOG(LogTemp, Warning, TEXT("Cocina: PatataSubsystem encontrado y asignado exitosamente."));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Cocina: Esperando a que PatataSubsystem sea inicializado..."));
    }
}

AActor* UZonaManager::GenerarZona()
{
	//int x, y;
	TArray<FVector2D> PosicionesPuentes;

	//if (mundoManager.Get()) {}

	//Hasta que obtenga la zona del metodo correspondiente, arrastrar esto
	TObjectPtr<AActor> zona = NULL;
	return zona;
}

