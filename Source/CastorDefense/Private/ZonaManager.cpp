#include "ZonaManager.h"
#include "Engine/Engine.h"

#include "PinguinoManager.h"
#include "MundoManager.h" 
#include "Zona.h"
#include "Nucleo.h"


//Se carga cuando existen todos los WolrdSubSystem
void UZonaManager::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

    MundoManager = InWorld.GetSubsystem<UMundoManager>();


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


FRotator UZonaManager::RotarPuentes(FVector2D PosicionCasilla) {
    FRotator Rotation;

    if (PosicionCasilla.X == (MundoManager->GetTamanyoLadoZona() - 1) || PosicionCasilla.X == 0) {
        if (PosicionCasilla.X == (MundoManager->GetTamanyoLadoZona() - 1)) {
            Rotation = FRotator(0.f, 0.f, -90.f);
        }
        else {
            Rotation = FRotator(0.f, 0.f, 90.f);
        }
    }
    else {
        if (PosicionCasilla.Y == (MundoManager->GetTamanyoLadoZona() - 1)) {
            Rotation = FRotator(0.f, 0.f, 180.f);
        }
        else {
            Rotation = FRotator(0.f, 0.f, 0.f);
        }
    }
    return Rotation;
}


AZona* UZonaManager::GenerarZona(FVector2D CoordenadasZona)
{
    int x = 0, y = 0;
    TArray<FVector2D> PosicionesPuentes;
    TArray<AZona*> ZonasAdyacentes;
    TArray<int> IndexZonasVacias;


    if (MundoManager->GetEstados().PosicionesPuentes.IsEmpty() && MundoManager->GetEstados().ZonaTerminada) {
        
        MundoManager->ComprobarZonasAdyacentes(CoordenadasZona, ZonasAdyacentes, IndexZonasVacias);
        //HAY Zonas Adyacentes
        if (!ZonasAdyacentes.IsEmpty()) {   
        
            for (AZona* item : ZonasAdyacentes) {

                PosicionesPuentes.Add(CalcularPosicionPuenteZonaPrevia(item,CoordenadasZona,FVector2D(0,0)));

                if (!item->GetInterseccion()) {
                    MundoManager->SetEstadosTieneQueSerInterseccion(true);
                }
            }
        }
        //HAY Zonas Vacias
        if (!IndexZonasVacias.IsEmpty()) {
        
            for (int item : IndexZonasVacias) {
              
              bool Correcto;
              FVector2D Movimiento;
              FVector2D PuenteZonaNueva;

              //Evitamos la generacion de puentes esquinados
              do {
              
                PuenteZonaNueva = CalcularPosicionPuenteZonaPrevia(nullptr,CoordenadasZona, ObtenerPosicionConIndexCasillasOZonas(item, MundoManager->GetTamanyoLadoMundo()));
                ComprobarGeneracionDePuenteEsquinado(PosicionesPuentes, PuenteZonaNueva, Correcto, Movimiento);
              
              } while (!Correcto);

               PosicionesPuentes.Add(PuenteZonaNueva);
              
            }
        }
       
         MundoManager->SetEstadosPosicionesPuentes(PosicionesPuentes);

    }

    //EMPIEZA EL PINGÜINO
    if (PinguinoManager->PinguinoBorracho(MundoManager->GetEstados().PosicionesPuentes, MundoManager->GetEstados().MapaAPintar)) {
    
        MundoManager->SetEstadosPosicionesPuentes(TArray<FVector2D>());

        ;
        //SPAWN ZONA
        AZona* Zona = GetWorld()->SpawnActorDeferred<AZona>(
			AZona::StaticClass(),
            FTransform (FRotator::ZeroRotator, FVector(CoordenadasZona.Y, CoordenadasZona.X, 0)),
            nullptr,
			nullptr,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );
            if(Zona){
				Zona->SetInterseccion(ComprobarSiHayInterseccion(CoordenadasZona, MundoManager->GetEstados().CantidadFinCiclos));

				MundoManager
                    ->SetEstadosTieneQueSerInterseccion(false)
                    ->SetEstadosCantidadFinCiclos(0);

				PintarMapaDeCasillas(MundoManager->GetEstados().MapaAPintar, Zona);

				MundoManager->SetEstadosMapaAPintar(TArray<ETipoCasilla>());
				Zona->SetActorHiddenInGame(true);
                Zona->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(CoordenadasZona.Y, CoordenadasZona.X, 0)));
            }
			
     
        return Zona;
    }

    return nullptr;
    
}


AZona* UZonaManager::GenerarZonaInicial(FVector2D CoordenadasZona)
{
    TArray<ETipoCasilla> MapaCasillasSimples;

    for (int y = 0; y < (MundoManager->GetTamanyoLadoZona() - 1); y++) {
        for (int x = 0; x < (MundoManager->GetTamanyoLadoZona() - 1); x++) {
            if (ComprobacionesPuentesEnZonaInicial(x, y, MundoManager->GetTamanyoLadoZona() - 1)) {
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
    //FALTA SpawnActorZonaInicial
    return nullptr;
}

AZona* UZonaManager::GenerarZonaPortal(FVector2D CoordenadasZona)
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
    for (int y = 0; y < (MundoManager->GetTamanyoLadoZona() - 1); y++) {
        for (int x = 0; x < (MundoManager->GetTamanyoLadoZona() - 1); x++) {
            MapaCasillasSimples.Add(ComprobacionesDeCaminoYPuenteEnZonasPortal(opcion, x, y));
        }
    }

    //FALTA SpawnactorZonaPortal

    return nullptr;
}

void UZonaManager::HabilitarZonas(AZona* Zona)
{
    Zona->SetActorHiddenInGame(false);

    for (const FDatosCasillas DatosCasilla : Zona->GetDatosCasillas()) {
        DatosCasilla.PuenteAsociado->SetActorHiddenInGame(false);
        if (!DatosCasilla.PuenteAsociado->GetConectadoCon()->IsHidden()) {
            DatosCasilla.PuenteAsociado->BajarPuente();
        }
    }
    TArray<AZona*> ZonasAdyacentes; 
    TArray<int> ZonasVacias;
    MundoManager->ComprobarZonasAdyacentes(FVector2D(Zona->GetActorLocation().Y, Zona->GetActorLocation().X), ZonasAdyacentes, ZonasVacias);
    for (AZona* ZonaAdyacente : ZonasAdyacentes) {
        if (!ZonaAdyacente->IsHidden()) {
            for (const FDatosCasillas DatosCasilla : ZonaAdyacente->GetDatosCasillas()) {
                if (Zona == DatosCasilla.PuenteAsociado->GetConectadoCon()) {
                   
                    DatosCasilla.PuenteAsociado->BajarPuente();
                }
            }
        }
    }
}

FVector2D UZonaManager::CalcularPosicionPuenteZonaPrevia(AZona* ZonaPrevia, FVector2D CoordenadasZonaNueva, FVector2D PosicionZonaPrevia)
{
    FVector2D CasillaPuenteZonaNueva;

    if (ZonaPrevia) {
    //Si Existe Zona

        FVector2D PosicionPuenteZonaPrevia = ZonaPrevia->GetDatosCasillasPuentes()[0].GridPosition;
		FVector2D Vector = FVector2D(CoordenadasZonaNueva.X - ZonaPrevia->GetActorLocation().Y, CoordenadasZonaNueva.Y - ZonaPrevia->GetActorLocation().X);
        //Calculamos donde estan los puentes de la ZonaPrevia
            if (Vector.X==0) {
            
                if (Vector.Y > 0) {
                   //El Puente de Zona Previa se encuentra Arriba, Crearemos el puente de la nueva Abajo
                    for (const FDatosCasillas& item : ZonaPrevia->GetDatosCasillasPuentes()) {
                    
                        if (item.GridPosition.Y > PosicionPuenteZonaPrevia.Y) {
                            PosicionPuenteZonaPrevia = item.GridPosition;
                        }
                    }
                  CasillaPuenteZonaNueva = FVector2D(PosicionPuenteZonaPrevia.X,0); 
                }
                else {
                    //El Puente de Zona Previa se encuentra Abajo, Crearemos el puente de la nueva Arriba
                    for (const FDatosCasillas& item : ZonaPrevia->GetDatosCasillasPuentes()) {

                        if (item.GridPosition.Y < PosicionPuenteZonaPrevia.Y) {
                            PosicionPuenteZonaPrevia = item.GridPosition;
                        }
                    }
                    CasillaPuenteZonaNueva = FVector2D(PosicionPuenteZonaPrevia.X, MundoManager->GetTamanyoLadoZona()-1);
                }
            }
            else {
            
                if (Vector.X > 0) {
                    //El Puente de Zona Previa se encuentra Derecha, Crearemos el puente de la nueva Izquierda
                    for (const FDatosCasillas& item : ZonaPrevia->GetDatosCasillasPuentes()) {

                        if (item.GridPosition.X > PosicionPuenteZonaPrevia.X) {
                            PosicionPuenteZonaPrevia = item.GridPosition;
                        }
                    }
                    CasillaPuenteZonaNueva = FVector2D(0,PosicionPuenteZonaPrevia.Y);
                
                }
                else {
                    //El Puente de Zona Previa se encuentra Izquierda, Crearemos el puente de la nueva Derecha
                    for (const FDatosCasillas& item : ZonaPrevia->GetDatosCasillasPuentes()) {

                        if (item.GridPosition.X < PosicionPuenteZonaPrevia.X) {
                            PosicionPuenteZonaPrevia = item.GridPosition;
                        }
                    }
                    CasillaPuenteZonaNueva = FVector2D(MundoManager->GetTamanyoLadoZona() - 1, PosicionPuenteZonaPrevia.Y);
                }
            
            }
    }
    else {
        bool PuenteCorrecto = true;
        do{
			//NO existe ZonaPrevia

            //Lado de una zona
			FVector2D ladoZona = VectorDeLadoZona();
			if (
				FVector2D(
					CoordenadasZonaNueva
					- (
						PosicionZonaPrevia
						* (ladoZona))
				).X == 0
				) {
				if (
					FVector2D(
						CoordenadasZonaNueva
						- (
							PosicionZonaPrevia
							* (ladoZona))
					).Y > 0
					) {
					CasillaPuenteZonaNueva = FVector2D(FMath::RandHelper(MundoManager->GetTamanyoLadoZona() - 2) + 1, 0.f);
				}
				else {
					CasillaPuenteZonaNueva = FVector2D(FMath::RandHelper(MundoManager->GetTamanyoLadoZona() - 2) + 1, MundoManager->GetTamanyoLadoZona() - 1);
				}
			}
			else {
				if (
					FVector2D(
						CoordenadasZonaNueva
						- (
							PosicionZonaPrevia
							* (ladoZona))
					).X > 0
					) {
					CasillaPuenteZonaNueva = FVector2D(0.f, FMath::RandHelper(MundoManager->GetTamanyoLadoZona() - 2) + 1);
				}
				else {
					CasillaPuenteZonaNueva = FVector2D(MundoManager->GetTamanyoLadoZona() - 1, FMath::RandHelper(MundoManager->GetTamanyoLadoZona() - 2) + 1);
				}
			}

			bool Diagonales;
			FVector2D PosicionZonaDiagonalANueva;
			BuscarZonasDiagonales(
                CasillaPuenteZonaNueva
                , (CoordenadasZonaNueva / FVector2D((MundoManager->GetTamanyoLadoZona() * MundoManager->GetTamanyoCasilla()) + (MundoManager->GetTamanyoCasilla() * 2)))
                , Diagonales
                , PosicionZonaDiagonalANueva
            );
			
            if (Diagonales) {
				AZona* Zona = MundoManager->GetMapaZonas()[ObtenerIndexConPosicionCasillasOZonas(PosicionZonaDiagonalANueva, MundoManager->GetTamanyoLadoMundo())];
				if (Zona) {
					for (const FDatosCasillas& DatosPuente : Zona->GetDatosCasillasPuentes()) {
						float Distancia = FVector2D::Distance(
                            ((DatosPuente.GridPosition * MundoManager->GetTamanyoCasilla()) + (PosicionZonaDiagonalANueva * ladoZona))
                            , ((CasillaPuenteZonaNueva * MundoManager->GetTamanyoCasilla()) + CoordenadasZonaNueva)
                        );
						if (
                            !(
                                Distancia > 
                                FMath::Sqrt(
                                    ((MundoManager->GetTamanyoCasilla() * 4) * (MundoManager->GetTamanyoCasilla() * 4))
                                    + ((MundoManager->GetTamanyoCasilla() * 4) * (MundoManager->GetTamanyoCasilla() * 4)))) //Pitagoras: Raiz de los Cuadrados de los catetos
                        ) {
                            PuenteCorrecto = false;
                        break;
						}
					}
				}
				else {
					return CasillaPuenteZonaNueva;
				}
			}
			else {
				return CasillaPuenteZonaNueva;
			}
        } while (PuenteCorrecto);
    }
    return CasillaPuenteZonaNueva;
}

void UZonaManager::ComprobarSiHayCasillasAdyacentes(FVector2D PosicionCasillas, TArray<ETipoCasilla> MapaSimple, TArray<FVector2D>& PCA, TArray<ETipoCasilla>& CasillasExistentesSimplificado)
{
    for (FVector2D Posicion : MundoManager->ObtenerPosiblesAdyacentes(PosicionCasillas, MundoManager->GetTamanyoLadoZona())) {
        if (MapaSimple[(Posicion.Y * MundoManager->GetTamanyoLadoZona()) + Posicion.X] == ETipoCasilla::NaN) {
            CasillasExistentesSimplificado.Add(MapaSimple[(Posicion.Y * MundoManager->GetTamanyoLadoZona()) + Posicion.X]);
            PCA.Add(Posicion);
        }
    }
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
        (Puentes.Contains(FVector2D(0, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2, 0.f)
        || (Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2, 0.f))) && PuenteNuevo == FVector2D(0.f, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2)
        || (Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 2, 0.f)
        || (Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 2, 0.f))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2)
        || (Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1))) && PuenteNuevo == FVector2D(0.f, MundoManager->GetTamanyoLadoZona() - 2)
        || (Puentes.Contains(FVector2D(0.f, MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1)
        || (Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1)
        || (Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 2)
        );
    if (!Correcto)
    {
        Correcto = true;
        if ((Puentes.Contains(FVector2D(0, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2, 0.f)) {
            MovimientoPosible = FVector2D(1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2, 0.f))) && PuenteNuevo == FVector2D(0.f, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2)) {
            MovimientoPosible = FVector2D(0.f, 1.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 2, 0.f)) {
            MovimientoPosible = FVector2D(-1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 2, 0.f))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 1 - MundoManager->GetTamanyoLadoZona() - 2)) {
            MovimientoPosible = FVector2D(0.f, 1.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1))) && PuenteNuevo == FVector2D(0.f, MundoManager->GetTamanyoLadoZona() - 2)) {
            MovimientoPosible = FVector2D(0.f, -1.f);
        }
        if ((Puentes.Contains(FVector2D(0.f, MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1)) {
            MovimientoPosible = FVector2D(1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 2))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1)) {
            MovimientoPosible = FVector2D(-1.f, 0.f);
        }
        if ((Puentes.Contains(FVector2D(MundoManager->GetTamanyoLadoZona() - 2, MundoManager->GetTamanyoLadoZona() - 1))) && PuenteNuevo == FVector2D(MundoManager->GetTamanyoLadoZona() - 1, MundoManager->GetTamanyoLadoZona() - 2)) {
            MovimientoPosible = FVector2D(0.f, -1.f);
        }
    }
}

void UZonaManager::PintarMapaDeCasillas(TArray<ETipoCasilla> MapaSimplificado, AZona* Zona)
{
    int Index = 0;

    for(ETipoCasilla& TipoCasilla : MapaSimplificado)
    {
        FVector2D Posicion = ObtenerPosicionConIndexCasillasOZonas(Index, MundoManager->GetTamanyoLadoZona());
        switch (TipoCasilla) {
            case ETipoCasilla::NaN:
            case ETipoCasilla::Terreno:
               
                Zona->GetDatosCasillas().Add(FDatosCasillas(ETipoCasilla::Terreno, Posicion, false, nullptr));
                
                break;
            case ETipoCasilla::Camino:
            case ETipoCasilla::CaminoConectado:

				Zona->GetDatosCasillas().Add(FDatosCasillas(ETipoCasilla::Camino, Posicion, false, nullptr));

                break;
            case ETipoCasilla::Puente:
            case ETipoCasilla::PuenteConectado:
            {
                FVector2D PosiciondelPuente = MundoManager->GetTamanyoCasilla() * Posicion + FVector2D(Zona->GetActorLocation().Y, Zona->GetActorLocation().X);

                APuente* Puente = GetWorld()->SpawnActor<APuente>(

                    AZona::StaticClass(),
                    FTransform(RotarPuentes(Posicion), FVector(PosiciondelPuente.Y, PosiciondelPuente.X, 0), FVector(2, 2, 2))

                );
                if (Puente) {

                    Puente->SetActorHiddenInGame(true);
                    Zona->GetDatosCasillas().Add(FDatosCasillas(ETipoCasilla::Puente, Posicion, false, Puente));
                    Zona->GetDatosCasillasPuentes().Add(FDatosCasillas(ETipoCasilla::Puente, Posicion, false, Puente));

                }
            }
                break;
            case ETipoCasilla::Objetivo:
            {
                Zona->GetDatosCasillas().Add(FDatosCasillas(ETipoCasilla::Objetivo, Posicion, false, nullptr));
                Posicion = (Posicion * MundoManager->GetTamanyoCasilla());

                ANucleo* Nucleo = GetWorld()->SpawnActor<ANucleo>(

                    ANucleo::StaticClass(),
                    FTransform(FRotator(), FVector(Posicion.Y + Zona->GetActorLocation().Y, Posicion.X + Zona->GetActorLocation().X, Zona->GetActorLocation().Z + 100), FVector(1, 1, 1))

                );
            }
                break;
            case ETipoCasilla::SpawnEnemigo:
                Zona->GetDatosCasillas().Add(FDatosCasillas(ETipoCasilla::SpawnEnemigo, Posicion, false, nullptr));
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
        || PosicionNuevoPuente.X == MundoManager->GetTamanyoLadoZona() - 2 
        || PosicionNuevoPuente.X == MundoManager->GetTamanyoLadoZona() - 2
        ) 
    {
        if (PosicionNuevoPuente.X == 1.f) {
            if (PosicionNuevoPuente.Y < MundoManager->GetTamanyoLadoZona() / 2) {
                ZonaDiagonalAComprobar += FVector2D(-1.f, -1.f);
            }
            else if (PosicionNuevoPuente.Y >= (MundoManager->GetTamanyoLadoZona() / 2) + 1) {
                ZonaDiagonalAComprobar += FVector2D(-1.f, 1.f);
            }
            else {
                return;
            }
        }
        else if (PosicionNuevoPuente.X < MundoManager->GetTamanyoLadoZona() / 2) {
            ZonaDiagonalAComprobar += FVector2D(1.f, -1.f);
        }
        else if (PosicionNuevoPuente.X >= (MundoManager->GetTamanyoLadoZona() / 2) + 1) {
            ZonaDiagonalAComprobar += FVector2D(1.f, 1.f);
        }
        else {
            return;
        }
    }
    else {
        return;
    }
    if(MundoManager->ComprobarLimitesDeMatriz(ZonaDiagonalAComprobar, MundoManager->GetTamanyoLadoMundo())){

        Diagonales = true;
        PosicionNuevaZona = ZonaDiagonalAComprobar;
    }
}

void UZonaManager::AsociarPuentesConZonas(TArray<AZona*> MapaZonas)
{
    int IndexASumar;

    for (AZona* item : MapaZonas) {
        int Index = 0;
        for (FDatosCasillas itemdatos : item->GetDatosCasillasPuentes()) {

            if (itemdatos.GridPosition.X == 0) {
                IndexASumar = -1;
            }
            else if(itemdatos.GridPosition.X == 10){
				IndexASumar = 1;
			}
			else if (itemdatos.GridPosition.Y == 0) {
				IndexASumar = MundoManager->GetTamanyoLadoMundo() * -1;
            }
            else {
                IndexASumar = MundoManager->GetTamanyoLadoMundo();
            }
            
            itemdatos.PuenteAsociado->SetConectadoCon(MapaZonas[Index + IndexASumar]);
            
        }
        Index++;
    }
}

FVector2D UZonaManager::VectorDeLadoZona()
{
  return FVector2D(MundoManager->GetTamanyoCasilla()* MundoManager->GetTamanyoLadoZona() + (MundoManager->GetTamanyoCasilla() * 2)
      , MundoManager->GetTamanyoCasilla() * MundoManager->GetTamanyoLadoZona() + (MundoManager->GetTamanyoCasilla() * 2));
}

ETipoCasilla UZonaManager::ComprobacionesDeCaminoYPuenteEnZonasPortal(int opcion, int X, int Y)
{
    bool NoEsTerreno = false;
    switch (opcion) {
    case 0:
        if ((X == 5 && Y >= 5) || (Y == 5 && X >= 5)) {
            NoEsTerreno = true;
        }
        break;
    case 1:
        if ((X == 5 && Y <= 5) || (Y == 5 && X >= 5)) {
            NoEsTerreno = true;
        }
        break;
    case 2:
        if ((X == 5 && Y >= 5) || (Y == 5 && X <= 5)) {
            NoEsTerreno = true;
        }
        break;
    case 3:
        if ((X == 5 && Y <= 5) || (Y == 5 && X <= 5)) {
            NoEsTerreno = true;
        }
        break;

    }
    if (NoEsTerreno) {
        if (ComprobacionesPuentesEnZonaInicial(X, Y, MundoManager->GetTamanyoLadoZona() - 1)) {
            return ETipoCasilla::Puente;
        }
        else if (X == 5 && Y == 5) {
            return ETipoCasilla::SpawnEnemigo;
        }
        else {
            return ETipoCasilla::Camino;
        }
    }
    return ETipoCasilla::Terreno;
}

bool UZonaManager::ComprobarSiHayInterseccion(FVector2D CoordenadasZona, int CantidadFinCilos)
{
    FVector2D Vector = CoordenadasZona / VectorDeLadoZona();

    if (MundoManager->GetTamanyoLadoMundo() - 1 == Vector.X
        && Vector.X == 0
        && MundoManager->GetTamanyoLadoMundo() - 1 == Vector.Y
        && Vector.Y == 0) {

        return true;
    }
    else if (CantidadFinCilos != 1) {

        return true;
    }
    else {
        return false;
    }
}
