// Fill out your copyright notice in the Description page of Project Settings.


#include "MundoManager.h"
#include "ZonaManager.h"
#include "Zona.h"

//Includes para el NavMesh
#include "Kismet/GameplayStatics.h"
//#include "NavigationSystemV1.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"


void UMundoManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	ZonaManager = InWorld.GetSubsystem<UZonaManager>();

}

UZonaManager* UMundoManager::GetZonaManager() {
    return ZonaManager;
}

//Probablemente borrable
void UMundoManager::SetZonaManager(UZonaManager* InZonaManager) {
    if (this->ZonaManager == InZonaManager) return;
    this->ZonaManager = InZonaManager;
}

TArray<AZona*> UMundoManager::GetMapaZonas()
{
    return this->MapaZonas;
}

void UMundoManager::SetMapaZonas(TArray<AZona*> InMapaZonas)
{
    this->MapaZonas = InMapaZonas;
}

float UMundoManager::GetTamanyoCasilla()
{
    return this->TamanyoCasilla;
}

void UMundoManager::SetTamanyoCasilla(float InTamanyoCasilla)
{
    this->TamanyoCasilla = InTamanyoCasilla;
}

int UMundoManager::GetTamanyoLadoZona()
{
    return this->TamanyoLadoZona;
}

void UMundoManager::SetTamanyoLadoZona(int InTamanyoLadoZona)
{
    this->TamanyoLadoZona = InTamanyoLadoZona;
}

float UMundoManager::GetTamanyoLadoMundo()
{
    return this->TamanyoLadoMundo;
}

void UMundoManager::SetTamanyoLadoMundo(int InTamanyoLadoMundo)
{
    this->TamanyoLadoMundo = InTamanyoLadoMundo;
}

FEstadosMundo UMundoManager::GetEstados()
{
    return this->Estados;
}

void UMundoManager::SetEstados(FEstadosMundo InEstados)
{
    this->Estados = InEstados;
}

UMundoManager* UMundoManager::SetEstadosZIndexCreados(TArray<int> InZIndexCreados)
{
    this->Estados.ZIndexCreados = InZIndexCreados;
    return this;
}

UMundoManager* UMundoManager::SetEstadosZIndexAdyacentes(TArray<int> InZIndexAdyacentes)
{
    this->Estados.ZIndexAdyacentes = InZIndexAdyacentes;
    return this;
}

UMundoManager* UMundoManager::SetEstadosZBuscarMas(bool InZBuscarMas)
{
    this->Estados.ZBuscarMas = InZBuscarMas;
    return this;
}

UMundoManager* UMundoManager::SetEstadosFin(bool InFin)
{
    this->Estados.Fin = InFin;
    return this;
}

UMundoManager* UMundoManager::SetEstadosTieneQueSerInterseccion(bool InTieneQueSerInterseccion)
{
	this->Estados.TieneQueSerInterseccion = InTieneQueSerInterseccion;
    return this;
}

UMundoManager* UMundoManager::SetEstadosMapaAPintar(TArray<ETipoCasilla> InMapaAPintar)
{
    this->Estados.MapaAPintar = InMapaAPintar;
    return this;
}

UMundoManager* UMundoManager::SetEstadosPosicionActual(FVector2D InPosicionActual)
{
    this->Estados.PosicionActual = InPosicionActual;
    return this;
}

UMundoManager* UMundoManager::SetEstadosMovimientoDinamico(TArray<FVector2D> InMovimientoDinamico)
{
    this->Estados.MovimientoDinamico = InMovimientoDinamico;
    return this;
}

UMundoManager* UMundoManager::SetEstadosCantidad(int InCantidad)
{
    this->Estados.Cantidad = InCantidad;
    return this;
}

UMundoManager* UMundoManager::SetEstadosZonaTerminada(bool InZonaTerminada)
{
    this->Estados.ZonaTerminada = InZonaTerminada;
    return this;
}

UMundoManager* UMundoManager::SetEstadosActually(uint8 InActually)
{
    this->Estados.Actually = InActually;
    return this;
}

UMundoManager* UMundoManager::SetEstadosPosicionesPuentes(TArray<FVector2D> InPosicionesPuentes)
{
    this->Estados.PosicionesPuentes = InPosicionesPuentes;
    return this;
}


UMundoManager* UMundoManager::SetEstadosCantidadFinCiclos(int InCantidadFinCiclos)
{
	this->Estados.CantidadFinCiclos = InCantidadFinCiclos;
    return this;
}

//Metodos

void UMundoManager::CrearNavMesh()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass());

	// 2. Comprobamos si existe y escalamos
	if (ANavMeshBoundsVolume* NavVolume = Cast<ANavMeshBoundsVolume>(FoundActor))
	{
        NavVolume->SetActorLocation(FVector(((((TamanyoLadoZona - 1) * 2) + (TamanyoLadoZona * TamanyoLadoMundo) * TamanyoCasilla) / 2), ((((TamanyoLadoZona - 1) * 2) + (TamanyoLadoZona * TamanyoLadoMundo) * TamanyoCasilla) / 2), 1.f));
		// Aplicamos la escala deseada
		NavVolume->SetActorScale3D(FVector(((((TamanyoLadoZona - 1) * 2) + (TamanyoLadoZona * TamanyoLadoMundo) * TamanyoCasilla) / TamanyoCasilla), ((((TamanyoLadoZona - 1) * 2) + (TamanyoLadoZona * TamanyoLadoMundo) * TamanyoCasilla) / TamanyoCasilla), 1.0f));
		
        UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

        if (NavSys)
        {
            // Informamos al sistema que los límites han cambiado. 
            // Esto dispara la regeneración de los polígonos del NavMesh.
            NavSys->OnNavigationBoundsUpdated(NavVolume);
        }
    }
}

AZona* UMundoManager::GenerarEsquinaMundo(int index)
{
    AZona* Zona = ZonaManager->GenerarZonaPortal(ObtenerCoordenadasDeUnaZonaConIndex(index));
    
    return Zona;
}

void UMundoManager::GenerarZonasDesdeInicio()
{   
    TArray<AZona*> ZonasAdyacentes;
    TArray<int> ZonasVacias;
    
    if (Estados.ZBuscarMas) {
        for (int item : Estados.ZIndexCreados) {
            FVector2D Coordenadas = ObtenerCoordenadasDeUnaZonaConIndex(item);
            ComprobarZonasAdyacentes(Coordenadas,ZonasAdyacentes,ZonasVacias);

            for (int item2 : ZonasVacias) {
                Estados.ZIndexAdyacentes.AddUnique(item2);
            }
        } 
    }

    if (Estados.ZIndexAdyacentes.IsEmpty()) {
        Estados.Fin = true;
    }
    else {

       // AZona* Zona = GetZonaManager()->GenerarZona(ObtenerCoordenadasDeUnaZonaConIndex(Estados.ZIndexAdyacentes[0]));
      //  if (Zona.IsDataValid()){
     //       MapaZonas[Estados.ZIndexAdyacentes[0]] = Zona;
        Estados.ZIndexCreados.Add(Estados.ZIndexAdyacentes[0]);
        Estados.ZIndexAdyacentes.Remove(Estados.ZIndexAdyacentes[0]);

        if (Estados.ZIndexAdyacentes.IsEmpty()) Estados.ZBuscarMas = true;

       }
    }



void UMundoManager::ComprobarZonasAdyacentes(FVector2D Coordenada, TArray<AZona*>& ZonasAdyacentes, TArray<int>& ZonasVacias)
{
    TArray<FVector2D> PosicionesPosibles = ObtenerPosiblesAdyacentes(FVector2D((TamanyoLadoZona * TamanyoCasilla) + (TamanyoCasilla * 2), (TamanyoLadoZona * TamanyoCasilla) + (TamanyoCasilla * 2)), TamanyoLadoMundo);
    
    for(FVector2D PosicionPosible : PosicionesPosibles){
      AZona* Zona = MapaZonas[PosicionPosible.X + (PosicionPosible.Y * TamanyoLadoMundo)];
      
      if(Zona){
          ZonasAdyacentes.Add(Zona);
      }else{
          ZonasVacias.Add(PosicionPosible.X + (PosicionPosible.Y * TamanyoLadoMundo));
      }
    }
   
}

TArray<FVector2D> UMundoManager::ObtenerPosiblesAdyacentes(FVector2D PosicionCentral, int TamanyoLadoMatriz)
{
    TArray<FVector2D> IndexPosibles;

    if(ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(1.f, 0.f), TamanyoLadoMatriz)){
        IndexPosibles.Add(PosicionCentral + FVector2D(1.f, 0.f));
    }
	if (ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(-1.f, 0.f), TamanyoLadoMatriz)) {
		IndexPosibles.Add(PosicionCentral + FVector2D(-1.f, 0.f));
	}
	if (ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(0.f, 1.f), TamanyoLadoMatriz)) {
		IndexPosibles.Add(PosicionCentral + FVector2D(0.f, 1.f));
	}
	if (ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(0.f, -1.f), TamanyoLadoMatriz)) {
		IndexPosibles.Add(PosicionCentral + FVector2D(0.f, -1.f));
	}

    return IndexPosibles;
}

bool UMundoManager::ComprobarLimitesDeMatriz(FVector2D Vector, int TamanyoLadoMatriz)
{
    return (
    Vector.X < TamanyoLadoMatriz    
    && Vector.X >= 0
    && Vector.Y < TamanyoLadoMatriz
	&& Vector.Y >= 0
    );
}

TArray<FVector2D> UMundoManager::ObtenerPosiblesDiagonalesAdyacentes(FVector2D PosicionCentral, int TamanyoLadoMatriz)
{

    TArray<FVector2D> IndexPosibles;

    if (ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(1.f, 1.f), TamanyoLadoMatriz)) {
        IndexPosibles.Add(PosicionCentral + FVector2D(1.f, 1.f));
    }
    if (ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(-1.f, 1.f), TamanyoLadoMatriz)) {
        IndexPosibles.Add(PosicionCentral + FVector2D(-1.f, 1.f));
    }
    if (ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(1.f, -1.f), TamanyoLadoMatriz)) {
        IndexPosibles.Add(PosicionCentral + FVector2D(1.f, -1.f));
    }
    if (ComprobarLimitesDeMatriz(PosicionCentral + FVector2D(-1.f, -1.f), TamanyoLadoMatriz)) {
        IndexPosibles.Add(PosicionCentral + FVector2D(-1.f, -1.f));
    }

    return IndexPosibles;
}
