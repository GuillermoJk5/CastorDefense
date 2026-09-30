// Fill out your copyright notice in the Description page of Project Settings.


#include "PinguinoManager.h"
#include "ZonaManager.h"
#include "TimerManager.h"

void UPinguinoManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            TimerHandle_AssignSubsystem,
            this,
            &UPinguinoManager::AsignarObjetos,
            0.2f,
            true
        );
    }
}

void UPinguinoManager::AsignarObjetos()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UZonaManager* FoundSubsystem = World->GetSubsystem<UZonaManager>();

    if (FoundSubsystem)
    {
        zonaManager = FoundSubsystem;
        World->GetTimerManager().ClearTimer(TimerHandle_AssignSubsystem);

        UE_LOG(LogTemp, Warning, TEXT("Cocina: PatataSubsystem encontrado y asignado exitosamente."));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Cocina: Esperando a que PatataSubsystem sea inicializado..."));
    }
}

bool UPinguinoManager::PinguinoBorracho(TArray<FVector2D> PosicionesPuentes, TArray <ETipoCasilla> MapaRecursivo) {

    return true;
}