// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/PinguinoManager.h"
#include "Managers/ZonaManager.h"
#include "Managers/MundoManager.h"
#include "TimerManager.h"

#include "Math/UnrealMathUtility.h"

void UPinguinoManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	ZonaManager = InWorld.GetSubsystem<UZonaManager>();
}

UZonaManager* UPinguinoManager::GetZonaManager() {
    return this->ZonaManager;
}

//Probablemente borrable
void UPinguinoManager::SetZonaManager(UZonaManager* InZonaManager) {
    if (this->ZonaManager == InZonaManager) return;
    this->ZonaManager = InZonaManager;
}

bool UPinguinoManager::PinguinoBorracho(TArray<FVector2D> PuentesRestantes, TArray<ETipoCasilla> MapaSimplificado) {

    TArray<FVector2D> ProbMovimientoEstatica = { FVector2D(0.f, 1.f), FVector2D(0.f, -1.f), FVector2D(1.f, 0.f), FVector2D(-1.f, 0.f) };
    ZonaManager->GetMundoManager()->SetEstadosZonaTerminada(true);
    if (MapaSimplificado.IsEmpty()) {
        //Si MapaRecursivo Viene Vacio: Resiceamos el Array,
        MapaSimplificado.Init(ETipoCasilla::NaN, ZonaManager->GetMundoManager()->GetTamanyoLadoZona() * ZonaManager->GetMundoManager()->GetTamanyoLadoZona());
        ZonaManager->GetMundoManager()->
            SetEstadosMapaAPintar(MapaSimplificado)->
            SetEstadosPosicionActual(PuentesRestantes[FMath::RandRange(0, PuentesRestantes.Num() - 1)]);

        for (FVector2D Puente : PuentesRestantes) {
            if (Puente == ZonaManager->GetMundoManager()->GetEstados().PosicionActual) {
                MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(Puente, ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                    = ETipoCasilla::PuenteConectado;
            }
            else {
                MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(Puente, ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                    = ETipoCasilla::Puente;
            }
        }
        //Comprobamos si hay puente esquinado
        bool Correcto;
        FVector2D MovimientoPosible;

        ZonaManager->ComprobarGeneracionDePuenteEsquinado(
            ObtenerTodosLosPuentesDeMapaAPintar(ZonaManager->GetMundoManager()->GetEstados().MapaAPintar)
            , ZonaManager->GetMundoManager()->GetEstados().PosicionActual
            , Correcto
            , MovimientoPosible
        );

        PuentesRestantes.Remove(
            ZonaManager->ObtenerPosicionConIndexCasillasOZonas(
                MapaSimplificado.Find(ETipoCasilla::PuenteConectado)
                , ZonaManager->GetMundoManager()->GetTamanyoLadoZona()
            )
        );

        ZonaManager->GetMundoManager()
            ->SetEstadosPosicionesPuentes(PuentesRestantes)
            ->SetEstadosMapaAPintar(MapaSimplificado)
            ->SetEstadosMovimientoDinamico(ProbMovimientoEstatica);

        if (!Correcto) {
            ZonaManager->GetMundoManager()->SetEstadosMovimientoDinamico(TArray<FVector2D>());
        }
    }
    else if (PuentesRestantes.Contains(ZonaManager->GetMundoManager()->GetEstados().PosicionActual)) {
        bool Correcto;
        FVector2D MovimientoPosible;

        ZonaManager->ComprobarGeneracionDePuenteEsquinado(
            ObtenerTodosLosPuentesDeMapaAPintar(ZonaManager->GetMundoManager()->GetEstados().MapaAPintar)
            , ZonaManager->GetMundoManager()->GetEstados().PosicionActual
            , Correcto
            , MovimientoPosible
        );

        MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(
            ZonaManager->GetMundoManager()->GetEstados().PosicionActual
            , ZonaManager->GetMundoManager()->GetTamanyoLadoZona()
        )]
            = ETipoCasilla::PuenteConectado;
        PuentesRestantes.Remove(ZonaManager->GetMundoManager()->GetEstados().PosicionActual);
        
        ZonaManager->GetMundoManager()->SetEstadosPosicionesPuentes(PuentesRestantes);

        if (!Correcto) {
            ZonaManager->GetMundoManager()->SetEstadosMovimientoDinamico(TArray<FVector2D>());
        }
    } 

    FVector2D PosicionAleatoriaCasillaNueva = DireccionDelPinguino(ZonaManager->GetMundoManager()->GetEstados().MovimientoDinamico, ZonaManager->GetMundoManager()->GetEstados().PosicionActual);

    int Index = ZonaManager->ObtenerIndexConPosicionCasillasOZonas(PosicionAleatoriaCasillaNueva, ZonaManager->GetMundoManager()->GetTamanyoLadoZona());

    if (ZonaManager->GetMundoManager()->ComprobarLimitesDeMatriz(PosicionAleatoriaCasillaNueva, ZonaManager->GetMundoManager()->GetTamanyoLadoZona())) {
        //Donde se va a crear hay ya casilla?? COMPROBACIONES
        TArray<FVector2D> PosicionesCasillasAdyacentes; //Posicion Casilla Adyacente
        TArray<ETipoCasilla> CasillasAdyacentesSimplificado;
        if (MapaSimplificado[Index] == ETipoCasilla::NaN) {
            //Comprobar la casilla Actual en busca de errores en el camino
            ZonaManager->ComprobarSiHayCasillasAdyacentes(ZonaManager->GetMundoManager()->GetEstados().PosicionActual, MapaSimplificado, PosicionesCasillasAdyacentes, CasillasAdyacentesSimplificado);
            
            if (
                ComprobarTipoMaxCasillasAdyacentes(
                    CasillasAdyacentesSimplificado
                    , 1
                    , ETipoCasilla::Camino
                )
            ) {
                //Comprobacion de la Nueva Casilla; Una casilla de camino solo puede tener 1 casilla de Camino antes de ser creada
                ZonaManager->ComprobarSiHayCasillasAdyacentes(PosicionAleatoriaCasillaNueva, MapaSimplificado, PosicionesCasillasAdyacentes, CasillasAdyacentesSimplificado);

                if (
                    ComprobarTipoMaxCasillasAdyacentes(
                        CasillasAdyacentesSimplificado
                        , 1
                        , ETipoCasilla::Camino
                    )
                    ) {
                    if (CasillasAdyacentesSimplificado.Contains(ETipoCasilla::CaminoConectado)) {
                        bool Correcto;
                        bool PuenteAdyacente;
                        ComprobarDestinoCamino(CasillasAdyacentesSimplificado, Correcto, PuenteAdyacente);
                        if (!Correcto) {
                            CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
                        }
                        else {
                            if (PuenteAdyacente) {
                                if (ComprobarTipoMaxCasillasAdyacentes(CasillasAdyacentesSimplificado, 1, ETipoCasilla::Puente)) {
                                    //ParteFinal
                                    //Si el destino es puente; Reasignamos Puente como PuenteConectado y lo borramos de Puentes Restantes
                                    UE_LOG(LogTemp, Warning, TEXT("X:%f, Y:%f"), ObtenerPosicionDestino(CasillasAdyacentesSimplificado, ETipoCasilla::Puente, PosicionesCasillasAdyacentes).X, ObtenerPosicionDestino(CasillasAdyacentesSimplificado, ETipoCasilla::Puente, PosicionesCasillasAdyacentes).Y);
                                    MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(ObtenerPosicionDestino(CasillasAdyacentesSimplificado, ETipoCasilla::Puente, PosicionesCasillasAdyacentes), ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                                        = ETipoCasilla::PuenteConectado;
                                    PuentesRestantes.Remove(ObtenerPosicionDestino(CasillasAdyacentesSimplificado, ETipoCasilla::Puente, PosicionesCasillasAdyacentes));
                                    ZonaManager->GetMundoManager()->SetEstadosPosicionesPuentes(PuentesRestantes);
                                    return HaciaFinPinguino(PuentesRestantes, MapaSimplificado, PosicionAleatoriaCasillaNueva);
                                }
                                else {
                                    CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
                                }
                            }
                            else {
                                //"Creamos" la casilla
                                MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(PosicionAleatoriaCasillaNueva, ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                                    = ETipoCasilla::Camino;

                                return HaciaFinPinguino(PuentesRestantes, MapaSimplificado, PosicionAleatoriaCasillaNueva);
                            }
                        }
                        
                    }
                    else if (CasillasAdyacentesSimplificado.Contains(ETipoCasilla::PuenteConectado)) {
                        //Comprobaciones de las casillas Adyacentes, en caso de que sean Destinos validos


                        if (!ComprobacionesPuenteConectado(MapaSimplificado, ObtenerPosicionDestino(CasillasAdyacentesSimplificado, ETipoCasilla::PuenteConectado, PosicionesCasillasAdyacentes))) {
                            CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
                            return false;
                        }
                        //"Creamos" la casilla
                        MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(PosicionAleatoriaCasillaNueva, ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                            = ETipoCasilla::Camino;

                        HaciaCrearCasillaYContinuar(ProbMovimientoEstatica, PosicionAleatoriaCasillaNueva, MapaSimplificado);
                    }
                    else if (CasillasAdyacentesSimplificado.Contains(ETipoCasilla::Puente)) {
                        if (ZonaManager->GetMundoManager()->GetEstados().PosicionesPuentes.Num() == 1 && ZonaManager->GetMundoManager()->GetEstados().TieneQueSerInterseccion) {
                            CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
                        }
                        if (ComprobarTipoMaxCasillasAdyacentes(CasillasAdyacentesSimplificado, 1, ETipoCasilla::Puente)) {
                            //ParteFinal
                            //Si el destino es puente; Reasignamos Puente como PuenteConectado y lo borramos de Puentes Restantes
                            MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(ObtenerPosicionDestino(CasillasAdyacentesSimplificado, ETipoCasilla::Puente, PosicionesCasillasAdyacentes), ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                                = ETipoCasilla::PuenteConectado;
                            PuentesRestantes.Remove(ObtenerPosicionDestino(CasillasAdyacentesSimplificado, ETipoCasilla::Puente, PosicionesCasillasAdyacentes));
                            ZonaManager->GetMundoManager()->SetEstadosPosicionesPuentes(PuentesRestantes);
                            //"Creamos" la casilla
                            MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(PosicionAleatoriaCasillaNueva, ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                                = ETipoCasilla::Camino;
                            return HaciaFinPinguino(PuentesRestantes, MapaSimplificado, PosicionAleatoriaCasillaNueva);
                        }
                        else {
                            CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
                        }
                    }
                    else {
                        //"Creamos" la casilla
                        MapaSimplificado[ZonaManager->ObtenerIndexConPosicionCasillasOZonas(PosicionAleatoriaCasillaNueva, ZonaManager->GetMundoManager()->GetTamanyoLadoZona())]
                            = ETipoCasilla::Camino;

                        HaciaCrearCasillaYContinuar(ProbMovimientoEstatica, PosicionAleatoriaCasillaNueva, MapaSimplificado);
                    }
                }
                else {
                    CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
                }
            }
            else {
                CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
            }
        }
        else {
            //Borra la casilla de camino anterior si vueve hacia atras
            if (MapaSimplificado[Index] == ETipoCasilla::Camino) {
                int IndexBorrado = ZonaManager->ObtenerIndexConPosicionCasillasOZonas(ZonaManager->GetMundoManager()->GetEstados().PosicionActual, ZonaManager->GetMundoManager()->GetTamanyoLadoZona());
                if (!( MapaSimplificado[IndexBorrado] == ETipoCasilla::Puente || MapaSimplificado[IndexBorrado] == ETipoCasilla::PuenteConectado )) {
                    MapaSimplificado[IndexBorrado] = ETipoCasilla::NaN;
                }
                HaciaCrearCasillaYContinuar(ProbMovimientoEstatica, PosicionAleatoriaCasillaNueva, MapaSimplificado);
            }
            else {
                CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
            }
        }
    }
    else {
        CalcularProbabilidadesDelMovimiento(PosicionAleatoriaCasillaNueva, MapaSimplificado);
    }
    return false;
}

TArray<FVector2D> UPinguinoManager::CambiarProbabilidadMovimiento(FVector2D CasillaNueva, FVector2D CasillaActual, TArray<FVector2D> DireccionDinamica)
{

    for (FVector2D item : DireccionDinamica) {
    
        if (item == (CasillaNueva - CasillaActual)) {
            
            DireccionDinamica.Remove(item);

            return DireccionDinamica;
        }
    
    }

    return DireccionDinamica;
}

FVector2D UPinguinoManager::DireccionDelPinguino(TArray<FVector2D> Probabilidad, FVector2D PosicionActual)
{
    return PosicionActual + Probabilidad[FMath::RandRange(0, Probabilidad.Num() - 1)];
     
}

bool UPinguinoManager::ComprobarTipoMaxCasillasAdyacentes(TArray<ETipoCasilla> DireccionDinamica, int MaximoDeCasillas, ETipoCasilla TipoDeCasilla)
{
    int CasillasDelTipo = 0;
    for (ETipoCasilla Item : DireccionDinamica) {
        if (Item == TipoDeCasilla) {
            CasillasDelTipo++;
        }
    }
    return CasillasDelTipo <= MaximoDeCasillas;
}

bool UPinguinoManager::ComprobacionesPuenteConectado(TArray<ETipoCasilla> MapaSimple, FVector2D PosicionPuente)
{
    TArray<ETipoCasilla> CasillasExistentesSimplificado;
    TArray<FVector2D> PosicionesCasillasAdyacentes;
    ZonaManager->ComprobarSiHayCasillasAdyacentes(PosicionPuente, MapaSimple,PosicionesCasillasAdyacentes ,CasillasExistentesSimplificado);
    return CasillasExistentesSimplificado.Num() == 0;
}

void UPinguinoManager::ComprobarDestinoCamino(TArray<ETipoCasilla> CasillasAdyacentesExistentes, bool& Bool, bool& PuenteAdyacente)
{
    
    if (CasillasAdyacentesExistentes.Contains(ETipoCasilla::Camino) && CasillasAdyacentesExistentes.Contains(ETipoCasilla::PuenteConectado)) {
        Bool = false;
        PuenteAdyacente = false;
    }
    else {
        uint8 Contador = 0;
        for (ETipoCasilla TipoCasillaAdyacente : CasillasAdyacentesExistentes) {
            if (TipoCasillaAdyacente == ETipoCasilla::CaminoConectado) {
                Contador++;
                if (Contador > 1) {
                    Bool = true;
                    PuenteAdyacente = CasillasAdyacentesExistentes.Contains(ETipoCasilla::Puente);
                    return;
                }
            }
        }
    }
}

bool UPinguinoManager::HaciaFinPinguino(TArray<FVector2D> PuentesRestantes, TArray<ETipoCasilla> MapaSimplificado, FVector2D PosicionAleatoriaNuevaCasilla)
{
    if (!PuentesRestantes.IsEmpty()) {
        int Index = 0;

        for (ETipoCasilla TipoCasillaIteracion : MapaSimplificado) {
            if (TipoCasillaIteracion == ETipoCasilla::Camino) {
                MapaSimplificado[Index] = ETipoCasilla::CaminoConectado;
            }
            Index++;
        }
        ZonaManager->GetMundoManager()
            ->SetEstadosMapaAPintar(MapaSimplificado)
            ->SetEstadosPosicionActual(PuentesRestantes[FMath::RandRange(0, PuentesRestantes.Num() - 1)])
            ->SetEstadosZonaTerminada(false)
            ->SetEstadosCantidadFinCiclos(ZonaManager->GetMundoManager()->GetEstados().CantidadFinCiclos + 1);
        return false;
    }
    else {
        ZonaManager->GetMundoManager()
            ->SetEstadosMapaAPintar(MapaSimplificado)
            ->SetEstadosZonaTerminada(true);
        return true;
    }
}

void UPinguinoManager::HaciaCrearCasillaYContinuar(TArray<FVector2D> ProbMovimientoEstatica,FVector2D PosicionAleatoriaNuevaCasilla,TArray<ETipoCasilla> MapaAPintar)
{
    if (ZonaManager->GetMundoManager()->GetEstados().Cantidad <= 1000) {
    
        ZonaManager->GetMundoManager()
            ->SetEstadosMapaAPintar(MapaAPintar)
            ->SetEstadosPosicionActual(PosicionAleatoriaNuevaCasilla)
            ->SetEstadosMovimientoDinamico(EliminarProbabilidadesMovimientosSiBordes(PosicionAleatoriaNuevaCasilla,ProbMovimientoEstatica))
            ->SetEstadosCantidad(ZonaManager->GetMundoManager()->GetEstados().Cantidad + 1)
            ->SetEstadosZonaTerminada(false);
    
    }
    else {

        BorrarYReiniciar();
    }
}

void UPinguinoManager::BorrarYReiniciar()
{
    ZonaManager->GetMundoManager()
        ->SetEstadosPosicionesPuentes(TArray<FVector2D>())
        ->SetEstadosMapaAPintar(TArray<ETipoCasilla>())
        ->SetEstadosCantidad(0)
        ->SetEstadosZonaTerminada(true)
        ->SetEstadosTieneQueSerInterseccion(false)
        ->SetEstadosCantidadFinCiclos(0);
}

void UPinguinoManager::CalcularProbabilidadesDelMovimiento(FVector2D PosicionAleatoriaNuevaCasilla, TArray<ETipoCasilla> MapaSimplificado)
{
    if (ZonaManager->GetMundoManager()->GetEstados().MovimientoDinamico.Num() <= 1) {
    
        BorrarYReiniciar();

    }else{    
        
        TArray<FVector2D> ProbMovimiento = CambiarProbabilidadMovimiento(PosicionAleatoriaNuevaCasilla, ZonaManager->GetMundoManager()->GetEstados().PosicionActual, ZonaManager->GetMundoManager()->GetEstados().MovimientoDinamico);

        ZonaManager->GetMundoManager()
            ->SetEstadosMapaAPintar(MapaSimplificado)
            ->SetEstadosMovimientoDinamico(ProbMovimiento)
            ->SetEstadosCantidad(ZonaManager->GetMundoManager()->GetEstados().Cantidad + 1)
            ->SetEstadosZonaTerminada(false);
       
    }
}

TArray<FVector2D> UPinguinoManager::ObtenerTodosLosPuentesDeMapaAPintar(TArray<ETipoCasilla> MapaSimplificado)
{
    TArray<FVector2D> PosicionesTodosPuentes;
    for (ETipoCasilla tipoCasilla : MapaSimplificado) {
        int Index = 0;
        if (tipoCasilla == ETipoCasilla::Puente || tipoCasilla == ETipoCasilla::PuenteConectado) {
            PosicionesTodosPuentes.Add(ZonaManager->ObtenerPosicionConIndexCasillasOZonas(Index, ZonaManager->GetMundoManager()->GetTamanyoLadoZona()));
            if (PosicionesTodosPuentes.Num() > 3) {
                return PosicionesTodosPuentes;
            }
        }
        Index++;
    }
    return TArray<FVector2D>();
}

TArray<FVector2D> UPinguinoManager::EliminarProbabilidadesMovimientosSiBordes(FVector2D PCI, TArray<FVector2D> Direcciones)
{
    //Elimina las posibilidades del movimiento del pinguino en caso de que el HIJO DE LA GRAN PUTA decida hacer bucles infinitos en putos bordes de los cojones
    if (PCI.X == ZonaManager->GetMundoManager()->GetTamanyoLadoZona() - 1) {
        Direcciones.Remove(FVector2D(1.f, 0.f));
    }
    else if (PCI.X == 0) {
        Direcciones.Remove(FVector2D(-1.f, 0.f));
    }

    if (PCI.Y == ZonaManager->GetMundoManager()->GetTamanyoLadoZona() - 1) {
        Direcciones.Remove(FVector2D(0.f, 1.f));
    }
    else if (PCI.Y == 0) {
        Direcciones.Remove(FVector2D(0.f, -1.f));
    }

    return Direcciones;
}
