// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MundoManager.h"
#include "Managers/ZonaManager.h"
#include "Mapa/Zonas/Zona.h"
#include "Managers/OleadasManager.h"
//#include "Managers/EnemigoManager.h"

//Includes para el NavMesh
#include "Kismet/GameplayStatics.h"

#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"

#include "Components/StaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Algo/Reverse.h"


UMundoManager::UMundoManager()
{
    if (!MeshCasilla) {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Mapa/Casillas/SM_Casilla.SM_Casilla"));

        if (MeshAsset.Succeeded())
        {
            MeshCasilla = MeshAsset.Object;
        }
    }
    this->TamanyoLadoMundo = 5;
    this->TamanyoLadoZona = 11;
    this->Estados.Fin = false;
    this->Estados.ZBuscarMas = true;
    this->Estados.TieneQueSerInterseccion = false;
}

void UMundoManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

    FBox BoundingBox = MeshCasilla->GetBoundingBox();

    TamanyoCasilla = (BoundingBox.GetSize().X) * 2.f;

	ZonaManager = InWorld.GetSubsystem<UZonaManager>();

    OleadasManager = InWorld.GetSubsystem<UOleadasManager>();

    InWorld.GetTimerManager().SetTimerForNextTick(this, &UMundoManager::Inicio);
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

UOleadasManager* UMundoManager::GetOleadasManager()
{
	return this->OleadasManager;
}

void UMundoManager::SetOleadasManager(UOleadasManager* InOleadasManager)
{
	this->OleadasManager = InOleadasManager;
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

void UMundoManager::Inicio()
{
    CrearNavMesh();
    MapaZonas.Init(nullptr, TamanyoLadoMundo * TamanyoLadoMundo);
    int indexCentral = ((TamanyoLadoMundo * TamanyoLadoMundo) - 1) / 2;

    //Generar Zona Inicial
    AZona* Zona = ZonaManager->GenerarZonaInicial(ObtenerCoordenadasDeUnaZonaConIndex(indexCentral));
    MapaZonas[indexCentral] = Zona;
    
    //Generar Zonas Portal
    GenerarEsquinasMundo();
	
    SetEstadosZIndexCreados(TArray<int> {indexCentral});

    //Generar Zonas Restantes
    do {

        GenerarZonasDesdeInicio();

        if (Estados.Actually > 50) {
            //AQUI SE HACE UN DELAY
            Estados.Actually = 0;
        }
        else {
            Estados.Actually++;
        }

    } while (Estados.Fin == false);

    ZonaManager->AsociarPuentesConZonas(MapaZonas);

    CalcularCantidadCasillasAInstanciar();
}

void UMundoManager::GenerarEsquinasMundo()
{
    const TArray<int> PosicionesEsquina = { 0,TamanyoLadoMundo - 1,TamanyoLadoMundo * TamanyoLadoMundo - TamanyoLadoMundo,TamanyoLadoMundo * TamanyoLadoMundo - 1 };
    for (int Posicion : PosicionesEsquina) {
        MapaZonas[Posicion] = ZonaManager->GenerarZonaPortal(ObtenerCoordenadasDeUnaZonaConIndex(Posicion));
    }
	  
}

void UMundoManager::ObtenerCasillaParaColocar(FVector LocalizacionPlayer, FDatosCasillas& Casilla, FVector2D& CoordenadasCasilla)
{
    //Comprobamos en que Zona esta el Player
    for (AZona* item :MapaZonas) {
        if (LocalizacionPlayer.X > item->GetActorLocation().X - TamanyoCasilla && LocalizacionPlayer.X < item->GetActorLocation().X - TamanyoCasilla + 2600 &&
            LocalizacionPlayer.Y > item->GetActorLocation().Y - TamanyoCasilla && LocalizacionPlayer.Y < item->GetActorLocation().Y - TamanyoCasilla + 2600)
        {
            //Comprobamos en que Casilla esta el Player
            for (FDatosCasillas datos : item->GetDatosCasillas()) {
            
                CoordenadasCasilla = FVector2D(item->GetActorLocation().X, item->GetActorLocation().Y) + FVector2D(datos.GridPosition.Y * TamanyoCasilla, datos.GridPosition.X * TamanyoCasilla);

                if(LocalizacionPlayer.X > CoordenadasCasilla.X - TamanyoCasilla/2 && LocalizacionPlayer.X < CoordenadasCasilla.X + TamanyoCasilla / 2 &&
                    LocalizacionPlayer.Y > CoordenadasCasilla.Y - TamanyoCasilla / 2 && LocalizacionPlayer.Y < CoordenadasCasilla.Y + TamanyoCasilla / 2)
                {
                    Casilla = datos;
                    break;
                }
            }

            break;
        }

    }
    
   
}

void UMundoManager::GenerarZonasDesdeInicio()
{
    TArray<AZona*> ZonasAdyacentes;
    TArray<int> ZonasVacias;

    if (Estados.ZBuscarMas) {
        for (int item : Estados.ZIndexCreados) {
            FVector2D Coordenadas = ObtenerCoordenadasDeUnaZonaConIndex(item);
            ComprobarZonasAdyacentes(Coordenadas, ZonasAdyacentes, ZonasVacias);
            for (int item2 : ZonasVacias) {
                Estados.ZIndexAdyacentes.AddUnique(item2);
            }
        }
        Estados.ZBuscarMas = false;
    }
    if (Estados.ZIndexAdyacentes.IsEmpty()) {
        Estados.Fin = true;
    }
    else {

        AZona* Zona = ZonaManager->GenerarZona(ObtenerCoordenadasDeUnaZonaConIndex(Estados.ZIndexAdyacentes[0]));
        if (Zona) {
            MapaZonas[Estados.ZIndexAdyacentes[0]] = Zona;
            Estados.ZIndexCreados.Add(Estados.ZIndexAdyacentes[0]);
            Estados.ZIndexAdyacentes.RemoveAt(0);

            if (Estados.ZIndexAdyacentes.IsEmpty()) Estados.ZBuscarMas = true;

        }
    }
}

void UMundoManager::ComprobarZonasAdyacentes(FVector2D Coordenada, TArray<AZona*>& ZonasAdyacentes, TArray<int>& ZonasVacias)
{
    TArray<FVector2D> PosicionesPosibles = ObtenerPosiblesAdyacentes(FVector2D(Coordenada.X/((TamanyoLadoZona * TamanyoCasilla) + (TamanyoCasilla * 2)), Coordenada.Y/((TamanyoLadoZona * TamanyoCasilla) + (TamanyoCasilla * 2))), TamanyoLadoMundo);
    
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

void UMundoManager::CalcularCantidadCasillasAInstanciar()
{
    for (AZona* Zona : MapaZonas) {
        TMap<ETipoCasilla, TArray<FTransform>> CantidadCasillasPorTipo;
        for (const FDatosCasillas& DatosCasilla : Zona->GetDatosCasillas()) {
            FTransform Transform;
            Transform.SetLocation(FVector(
                (DatosCasilla.GridPosition.Y * TamanyoCasilla + TamanyoCasilla * 0.5f)
                , (DatosCasilla.GridPosition.X * TamanyoCasilla + TamanyoCasilla * 0.5f) 
                , TamanyoCasilla * 0.5f) * 0.5f
            );
            TArray<FTransform>& Transforms = CantidadCasillasPorTipo.FindOrAdd(DatosCasilla.Tipo);
            Transforms.Add(Transform);
        }
        for (const TPair<ETipoCasilla, TArray<FTransform>>& LLaveValor : CantidadCasillasPorTipo) {
            GuardarEInstanciarCasillas(LLaveValor.Key, LLaveValor.Value, Zona);
        }
    }
}

void UMundoManager::GuardarEInstanciarCasillas(ETipoCasilla Tipo, TArray<FTransform> Cantidad, AZona* Zona)
{
    UHierarchicalInstancedStaticMeshComponent* HIDeZona = *Zona->GetHI().Find(Tipo);
    
    if (IsValid(HIDeZona))
    {
        TArray<int> Index = HIDeZona->AddInstances(Cantidad, true);
        Algo::Reverse(Index);

        for (FDatosCasillas& DatosCasilla : Zona->GetDatosCasillas()) {
            if (DatosCasilla.Tipo == Tipo) {
                DatosCasilla.IndexLocal = Index.Pop();
                DatosCasilla.HIDuenyo = HIDeZona;
                if (Index.Num() <= 0) {
                    
                    return;
                }
            }
        }
    }
}

bool UMundoManager::ShouldCreateSubsystem(UObject* Outer) const
{
    TArray<UClass*> ChildClasses;
    GetDerivedClasses(GetClass(), ChildClasses, false);

    return ChildClasses.Num() == 0;
}
