#include "ZonaManager.h"
#include "Engine/Engine.h"

#include "PinguinoManager.h"
#include "MundoManager.h" 

//Se carga cuando existe ZonaManager (pudiendo no estar el resto) Eliminable
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

//Se carga cuando existen todos los WolrdSubSystem
void UZonaManager::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

    MundoManager = InWorld.GetSubsystem<UMundoManager>();
}

//Eliminable
void UZonaManager::AsignarObjetos()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UPinguinoManager* FoundSubsystem = World->GetSubsystem<UPinguinoManager>();

    if (FoundSubsystem)
    {
        PinguinoManager = FoundSubsystem;
        World->GetTimerManager().ClearTimer(TimerHandle_AssignSubsystem);

        UE_LOG(LogTemp, Warning, TEXT("Cocina: PatataSubsystem encontrado y asignado exitosamente."));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Cocina: Esperando a que PatataSubsystem sea inicializado..."));
    }
}

UMundoManager* UZonaManager::GetMundoManager() {
    return MundoManager;
}

//Probablemente borrable
void UZonaManager::SetMundoManager(UMundoManager* InMundoManager) {
    if (this->MundoManager == InMundoManager) return;
    this->MundoManager = InMundoManager;
}

UPinguinoManager* UZonaManager::GetPinguinoManager() {
    return PinguinoManager;
}

//Probablemente borrable
void UZonaManager::SetPinguinoManager(UPinguinoManager* InPinguinoManager) {
    if (this->PinguinoManager == InPinguinoManager) return;
    this->PinguinoManager = InPinguinoManager;
}

int UZonaManager::InstanciarCasilla(/*¿StaticMesh?* HIX, */FVector2D PosicionCasilla){
    /*
    MundoManager->casilla.AddInstance(PosicionCasilla.Y*(MundoManager->TamanyoCasilla/2),PosicionCasilla.X*(MundoManager->TamanyoCasilla/2))
    
    */
    return 0;
}

//FALTA PUENTE
void UZonaManager::RotarPuentes(/*UPuente* Puente, */FVector2D PosicionCasilla) {
    FRotator Rotation;

    if (PosicionCasilla.X == (MundoManager->TamanyoLadoZona - 1) || PosicionCasilla.X == 0) {
        if (PosicionCasilla.X == (MundoManager->TamanyoLadoZona - 1)) {
            Rotation = FRotator(0.f, 0.f, -90.f);
        }
        else {
            Rotation = FRotator(0.f, 0.f, 90.f);
        }
    }
    else {
        if (PosicionCasilla.Y == (MundoManager->TamanyoLadoZona - 1)) {
            Rotation = FRotator(0.f, 0.f, 180.f);
        }
        else {
            Rotation = FRotator(0.f, 0.f, 0.f);
        }
    }
    //Puente->SetActorRotation(Rotation, false);
}

//FALTA COMPROBARZONASADYACENTES
//FALTA PINGUINOBORRACHO
/*
AActor* UZonaManager::GenerarZona()
{
    int x = 0, y = 0;
    TArray<FVector2D> PosicionesPuentes;

    if (MundoManager->Estados.PosicionesPuentes.IsEmpty() && MundoManager->Estados.ZonaTerminada) {
        //MundoManager->ComprobarZonasAdyacentes();
    }

    //PinguinoManager->PinguinoBorracho();

    //Hasta que obtenga la zona del metodo correspondiente, arrastrar esto
    TObjectPtr<AActor> Zona = NULL;
    return Zona;
}
*/

AActor* UZonaManager::GenerarZonaInicial(FVector2D CoordenadasZona)
{
    TArray<ETipoCasilla> MapaCasillasSimples;
    //ETipoCasilla TipoCasilla;

    for (int y = 0; y < (MundoManager->TamanyoLadoZona - 1); y++) {
        for (int x = 0; x < (MundoManager->TamanyoLadoZona - 1); x++) {
            if (ComprobacionesPuentesEnZonaInicial(x, y, MundoManager->TamanyoLadoZona - 1)) {
                MapaCasillasSimples.Add(ETipoCasilla::Puente);
            }
            else if (x == 5 || y == 5) {
                if (x == 5 && y == 5) {
                    MapaCasillasSimples.Add(ETipoCasilla::Objetivo);
                }
                else {
                    MapaCasillasSimples.Add(ETipoCasilla::Camino);
                }
            }
            else {
                MapaCasillasSimples.Add(ETipoCasilla::Terreno);
            }
        }
    }
    //SpawnActorZonaInicial
    return nullptr;
}

AActor* UZonaManager::GenerarZonaPortal(FVector2D CoordenadasZona)
{
    int opcion;
    TArray<ETipoCasilla> MapaCasillasSimples;

    if (CoordenadasZona.X == 0) {
        if (CoordenadasZona.Y == 0) {
            opcion = 0;
        }
        else {
            opcion = 1;
        }
    }
    else {
        if (CoordenadasZona.Y == 0) {
            opcion = 2;
        }
        else {
            opcion = 3;
        }
    }
    for (int y = 0; y < (MundoManager->TamanyoLadoZona - 1); y++) {
        for (int x = 0; x < (MundoManager->TamanyoLadoZona - 1); x++) {
            MapaCasillasSimples.Add(ComprobacionesDeCaminoYPuenteEnZonasPortal(opcion, x, y));
        }
    }

    //SpawnactorZonaPortal

    return nullptr;
}

void UZonaManager::ComprobarSiHayCasillasAdyacentes(FVector2D PosicionCasillas, TArray<ETipoCasilla> MapaSimple, FVector2D& PCA, TArray<ETipoCasilla>& CasillasExistentesSimplificado)
{
    //TArray<FVector2D> PosicionesPosibles = MundoManager->ObtenerPosiblesAdyacentes(PosicionCasillas, MundoManager->TamanyoLadoZona);
    PCA;
    CasillasExistentesSimplificado;
}

int UZonaManager::ObtenerIndexConPosicionCasillasOZonas(FVector2D Posicion, int TamanyoLadoArray)
{
    return FMath::TruncToInt((Posicion.Y * TamanyoLadoArray) + Posicion.X);
}

FVector2D UZonaManager::ObtenerPosicionConIndexCasillasOZonas(int Index, int TamanyoLadoArray)
{
    return FVector2D(Index % TamanyoLadoArray, Index / TamanyoLadoArray);
}

void UZonaManager::ComprobarGeneracionDePuenteEsquinado(TArray<FVector2D> Puentes, FVector2D PuenteNuevo, bool& Correcto, FVector2D& MovimientoPosible)
{
    Correcto = !(
        (Puentes.Contains(FVector2D(0, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2, 0.f)
        || (Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2, 0.f))) && PuenteNuevo == FVector2D(0.f, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2)
        || (Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 2, 0.f)
        || (Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 2, 0.f))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2)
        || (Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1))) && PuenteNuevo == FVector2D(0.f, MundoManager->TamanyoLadoZona - 2)
        || (Puentes.Contains(FVector2D(0.f, MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1)
        || (Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1)
        || (Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 2)
        );
    if (!Correcto)
    {
        Correcto = true;
        if ((Puentes.Contains(FVector2D(0, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2, 0.f)) {
            MovimientoPosible = FVector2D(1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2, 0.f))) && PuenteNuevo == FVector2D(0.f, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2)) {
            MovimientoPosible = FVector2D(0.f, 1.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 2, 0.f)) {
            MovimientoPosible = FVector2D(-1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 2, 0.f))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 1 - MundoManager->TamanyoLadoZona - 2)) {
            MovimientoPosible = FVector2D(0.f, 1.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1))) && PuenteNuevo == FVector2D(0.f, MundoManager->TamanyoLadoZona - 2)) {
            MovimientoPosible = FVector2D(0.f, -1.f);
        }
        if ((Puentes.Contains(FVector2D(0.f, MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1)) {
            MovimientoPosible = FVector2D(1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 2))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1)) {
            MovimientoPosible = FVector2D(-1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->TamanyoLadoZona - 2, MundoManager->TamanyoLadoZona - 1))) && PuenteNuevo == FVector2D(MundoManager->TamanyoLadoZona - 1, MundoManager->TamanyoLadoZona - 2)) {
            MovimientoPosible = FVector2D(0.f, -1.f);
        }
    }
}

void UZonaManager::PintarMapaDeCasillas(TArray<ETipoCasilla> MapaSimplificado/*, UZona Zona*/)
{
    int Index = 0;

    for(ETipoCasilla& TipoCasilla : MapaSimplificado)
    {
        FVector2D Posicion = ObtenerPosicionConIndexCasillasOZonas(Index, MundoManager->TamanyoLadoZona);
        switch (TipoCasilla) {
            case ETipoCasilla::NaN:
            case ETipoCasilla::Terreno:
                break;
            case ETipoCasilla::Camino:
            case ETipoCasilla::CaminoConectado:
                break;
            case ETipoCasilla::Puente:
            case ETipoCasilla::PuenteConectado:
                break;
            case ETipoCasilla::Objetivo:
                break;
            case ETipoCasilla::SpawnEnemigo:
                break;
        }
        Index++;
    }
}

void UZonaManager::BuscarZonasDiagonales(FVector2D PosicionNuevoPuente, FVector2D PosicionNuevaZona, bool& Diagonales, FVector2D& PosicionZonaDiagonalANueva)
{
    FVector2D ZonaDiagonalAComprobar = PosicionNuevaZona;
    Diagonales = false;
    
    if (
        PosicionNuevoPuente.X == 1.f 
        || PosicionNuevoPuente.Y == 1.f 
        || PosicionNuevoPuente.X == MundoManager->TamanyoLadoZona - 2 
        || PosicionNuevoPuente.X == MundoManager->TamanyoLadoZona - 2
        ) 
    {
        if (PosicionNuevoPuente.X == 1.f) {
            if (PosicionNuevoPuente.Y < MundoManager->TamanyoLadoZona / 2) {
                ZonaDiagonalAComprobar += FVector2D(-1.f, -1.f);
            }
            else if (PosicionNuevoPuente.Y >= (MundoManager->TamanyoLadoZona / 2) + 1) {
                ZonaDiagonalAComprobar += FVector2D(-1.f, 1.f);
            }
            else {
                return;
            }
        }
        else if (PosicionNuevoPuente.X < MundoManager->TamanyoLadoZona / 2) {
            ZonaDiagonalAComprobar += FVector2D(1.f, -1.f);
        }
        else if (PosicionNuevoPuente.X >= (MundoManager->TamanyoLadoZona / 2) + 1) {
            ZonaDiagonalAComprobar += FVector2D(1.f, 1.f);
        }
        else {
            return;
        }
    }
    else {
        return;
    }
    //if(MundoManager->ComprobarLimitesDeMatriz(ZonaDiagonalAComprobar, MundoManager->TamanyoLadoMundo)){Diagonales = true;PosicionNuevaZona = ZonaDiagonalAComprobar;}
}

void UZonaManager::AsociarPuentesConZonas(/*TArray<UZona> Zona*/)
{
}


