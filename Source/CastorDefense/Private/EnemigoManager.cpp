// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemigoManager.h"
#include "MundoManager.h"
#include "ZonaManager.h"
#include "Nucleo.h"
#include "Enemigo.h"
#include "Zona.h"

#include "ClaseDeUtilidades.h"
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"
#include "TiposDeDatosComunes/Struct/FMovimientoEnemigo.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "Engine/Engine.h"

void UEnemigoManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	MundoManager = InWorld.GetSubsystem<UMundoManager>();
}

UMundoManager* UEnemigoManager::GetMundoManager()
{
	return this->MundoManager;
}

void UEnemigoManager::SetMundoManager(UMundoManager* InMundoManager)
{
	this->MundoManager = InMundoManager;
}


TArray<AEnemigo*> UEnemigoManager::GetEnemigosGestionables()
{
	return this->EnemigosGestionables;
}

void UEnemigoManager::SetEnemigosGestionables(TArray<AEnemigo*> InEnemigosGestionables)
{
	this->EnemigosGestionables = InEnemigosGestionables;
}

void UEnemigoManager::ActivarEnemigo(AZona* Zona, FDatosCasillas CasillaSpawn)
{

	ANucleo* Nucleo = Cast<ANucleo>(UGameplayStatics::GetActorOfClass(GetWorld(), ANucleo::StaticClass()));

	if (Nucleo)
	{
	TArray<FDatosCasillas> Ruta = CalcularCaminoASeguir(FVector2D(Nucleo->GetActorLocation().Y, Nucleo->GetActorLocation().X), Zona, CasillaSpawn);
	
	//FALTA NO HARDCODEAR ESTO
		if (Ruta.Contains(FDatosCasillas(0, ETipoCasilla::Objetivo, FVector2D(5, 5), *MundoManager->GetMapaZonas()[12]->GetHI().Find(ETipoCasilla::Objetivo), false, nullptr))) {
	
			PosicionarEnemigo(Ruta, CasillaSpawn, Zona);
		}
	}

}

TArray<FDatosCasillas> UEnemigoManager::CalcularCaminoASeguir(FVector2D CoordenadasObjetivo, AZona* ZonaSpawn, FDatosCasillas DatosCasillaActual)
{
	TMap<FDatosCasillas,FDatosCasillas>CaminoFin;
	
	
	TArray<FMovimientoEnemigo> CaminosPosibles;
	CaminosPosibles.Add(FMovimientoEnemigo(0, 0, 0, DatosCasillaActual, DatosCasillaActual, ZonaSpawn));
	FMovimientoEnemigo STConFMenor (0,0,0,DatosCasillaActual,FDatosCasillas(),ZonaSpawn);
	
	
	while (
		FVector2D(
			FVector2D(STConFMenor.CasillaActual.GridPosition * MundoManager->GetTamanyoCasilla() + FVector2D(STConFMenor.ZonaPerteneciente->GetActorLocation().Y, STConFMenor.ZonaPerteneciente->GetActorLocation().X)).Y,
			FVector2D(STConFMenor.CasillaActual.GridPosition * MundoManager->GetTamanyoCasilla() + FVector2D(STConFMenor.ZonaPerteneciente->GetActorLocation().Y, STConFMenor.ZonaPerteneciente->GetActorLocation().X)).X
		)
		!=
		CoordenadasObjetivo
		&&
		CaminosPosibles.Num() > 0) {

		for (FMovimientoEnemigo item : CaminosPosibles) {

			FVector2D NuevaCoordenada = item.CasillaActual.GridPosition * MundoManager->GetTamanyoCasilla() + FVector2D(STConFMenor.ZonaPerteneciente->GetActorLocation().Y, STConFMenor.ZonaPerteneciente->GetActorLocation().X);

			
			item.DistanciaAlObjetivo = FMath::TruncToInt(FMath::Abs(NuevaCoordenada.X - CoordenadasObjetivo.X) + FMath::Abs(NuevaCoordenada.Y - CoordenadasObjetivo.Y));
			item.CosteMovimiento = (item.CantidadRecorrida + item.DistanciaAlObjetivo);

			if (item.CosteMovimiento < STConFMenor.CosteMovimiento) {
				STConFMenor = item;
			}
		}


		if (!(STConFMenor.CasillaActual.Tipo == ETipoCasilla::Puente) && !(STConFMenor.CasillaAnterior.Tipo == ETipoCasilla::Puente) || CaminoFin.IsEmpty()) {

			CaminoFin.Add(STConFMenor.CasillaActual, STConFMenor.CasillaAnterior);
			STConFMenor.CantidadRecorrida++;
		}
		else {

			
			FDatosCasillas PuenteActual(
				STConFMenor.CasillaAnterior.IndexLocal + 4, 
				STConFMenor.CasillaAnterior.Tipo,
				FVector2D(
					STConFMenor.CasillaAnterior.GridPosition 
					+ 
					FVector2D(
						(
						FVector2D(STConFMenor.CasillaAnterior.HIDuenyo->GetOwner()->GetActorLocation().Y
								,STConFMenor.CasillaAnterior.HIDuenyo->GetOwner()->GetActorLocation().X)
						,FVector2D(STConFMenor.CasillaActual.HIDuenyo->GetOwner()->GetActorLocation().Y
								,STConFMenor.CasillaActual.HIDuenyo->GetOwner()->GetActorLocation().X)).GetSafeNormal())),
						
				STConFMenor.CasillaAnterior.HIDuenyo, 
				false, nullptr);

			//SemiPuente 1
			CaminoFin.Add(PuenteActual, STConFMenor.CasillaAnterior);

			FDatosCasillas PuenteAnterior = PuenteActual;

			PuenteActual.IndexLocal = STConFMenor.CasillaActual.IndexLocal + 4;
			PuenteActual.Tipo = STConFMenor.CasillaAnterior.Tipo;
			PuenteActual.GridPosition = FVector2D(STConFMenor.CasillaActual.GridPosition +
				FVector2D(
					(FVector2D(STConFMenor.CasillaActual.HIDuenyo->GetOwner()->GetActorLocation().Y
						, STConFMenor.CasillaActual.HIDuenyo->GetOwner()->GetActorLocation().X)
						,
						FVector2D(STConFMenor.CasillaAnterior.HIDuenyo->GetOwner()->GetActorLocation().Y
							, STConFMenor.CasillaAnterior.HIDuenyo->GetOwner()->GetActorLocation().X)).GetSafeNormal()));
										
			PuenteActual.HIDuenyo = STConFMenor.CasillaActual.HIDuenyo;
			PuenteActual.PuenteAsociado = nullptr;
			//SemiPuente2
			CaminoFin.Add(PuenteActual, PuenteAnterior);

			//Puente2
			CaminoFin.Add(STConFMenor.CasillaActual, PuenteActual);

			STConFMenor.CantidadRecorrida = STConFMenor.CantidadRecorrida + 3;
		}
		// Valorar Filtrar por predicado CaminosPosibles.FilterByPredicate()

		for(FMovimientoEnemigo CaminoPosible : CaminosPosibles){
			if(
				CaminoPosible.CasillaActual.IndexLocal == STConFMenor.CasillaActual.IndexLocal
			&& CaminoPosible.CasillaActual.HIDuenyo == STConFMenor.CasillaActual.HIDuenyo
			){
				CaminosPosibles.Remove(CaminoPosible);
				break;
			}
		}

		STConFMenor.CosteMovimiento = TNumericLimits<int>::Max();

		TArray<FDatosCasillas> Casillas;

		TMap <FDatosCasillas, AZona*> CasillasConZonas = ComprobarCaminosAdyacentes(STConFMenor.ZonaPerteneciente, STConFMenor.CasillaActual.GridPosition, CaminoFin);
		CasillasConZonas.GetKeys(Casillas);

		for(FDatosCasillas CasillaActual : Casillas){
			CaminosPosibles.Add(FMovimientoEnemigo(STConFMenor.CantidadRecorrida, 0, 0, CasillaActual, STConFMenor.CasillaActual, *CasillasConZonas.Find(CasillaActual)));
			//Actually
		}
		
	}

	bool Fin = true;
	TMap<FDatosCasillas, FDatosCasillas> DatosCasillaCamino;
	TArray<FDatosCasillas> Llaves;
	TArray<FDatosCasillas> RutaARellenar;

	CaminoFin.GetKeys(Llaves);
	DatosCasillaCamino.Add(Llaves.Last(), CaminoFin[Llaves.Last()]);

	while(Fin){
		RutaARellenar.Add(Llaves[0]);
		if(CaminoFin.Find(CaminoFin[Llaves[0]])){
			DatosCasillaCamino.Add(CaminoFin[Llaves[0]], *CaminoFin.Find(CaminoFin[Llaves[0]]));
			DatosCasillaCamino.Remove(Llaves[0]);

			TArray<FDatosCasillas> LlavesDatosCasillas;
			DatosCasillaCamino.GetKeys(LlavesDatosCasillas);

			if(DatosCasillaCamino.Find(LlavesDatosCasillas[0])){
				Fin = false;
				//Actually
			}
		}
	}
	
	return RutaARellenar;
}

TMap<FDatosCasillas, AZona*> UEnemigoManager::ComprobarCaminosAdyacentes(AZona* Zona, FVector2D Posicion, TMap<FDatosCasillas, FDatosCasillas> CaminoFinal)
{
	TMap<FVector2D, AZona*> IndexPosibles;
	bool Agregar;
	FVector2D PosicionesPosibles;
	AZona* ZonaAAgregar;
	for(int Index = 0 ; Index < 4 ; Index++){
		ComprobarAdyacentesYX(
			Posicion
			, (Index % 2 == 0 ? 1 : -1)
			, (Index % 2 == 0 ? 0 : MundoManager->GetTamanyoLadoZona() - 1)
			, Zona, (Index <= 1 ? false : true)
			, Agregar
			, ZonaAAgregar
			, PosicionesPosibles
		);
		if(Agregar){
			IndexPosibles.Add(PosicionesPosibles, ZonaAAgregar);
		}
	}

	TMap<FDatosCasillas, AZona*> CaminoDeCasillas;
	TArray<FVector2D> Indexes;
	IndexPosibles.GetKeys(Indexes);

	for( FVector2D PosicionPosible : Indexes){
		TArray<FDatosCasillas> Datos;
		CaminoFinal.GetKeys(Datos);
		if (
			!(Datos.Contains((*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X]))
			&& (*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X].Tipo == ETipoCasilla::Puente
			) {
			if((*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X].PuenteAsociado->GetBajado()){
				CaminoDeCasillas.Add((*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X], *IndexPosibles.Find(PosicionPosible));
			}
		}
		else {
			if (
				!(Datos.Contains((*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X]))
				&& ((*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X].Tipo == ETipoCasilla::Camino
				|| (*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X].Tipo == ETipoCasilla::Objetivo)
			) {
				CaminoDeCasillas.Add((*IndexPosibles.Find(PosicionPosible))->GetDatosCasillas()[PosicionPosible.Y * MundoManager->GetTamanyoLadoZona() + PosicionPosible.X], *IndexPosibles.Find(PosicionPosible));
			}
		}
	}
	
	return CaminoDeCasillas;
}


void UEnemigoManager::ComprobarAdyacentesYX(FVector2D PosicionCentral, float Valor, float ValorComprobacionContraria, AZona* Zona, bool YX, bool& Agregar, AZona*& Z, FVector2D& PosicionPosible)
{
	auto SetDatosADevolver = [&](AZona* InZona, FVector2D InPosicionPosible, bool InAgregar)
	{
			Z = InZona;
			PosicionPosible = InPosicionPosible;
			Agregar = InAgregar;
	};
	
	FVector2D VectorYX1 = YX ? FVector2D(PosicionCentral.X, PosicionCentral.Y + Valor) : FVector2D(PosicionCentral.X + Valor, PosicionCentral.Y);
	
	if(!MundoManager->ComprobarLimitesDeMatriz(VectorYX1, MundoManager->GetTamanyoLadoZona())){
		if(Zona->GetDatosCasillas()[MundoManager->GetZonaManager()->ObtenerIndexConPosicionCasillasOZonas(PosicionCentral, MundoManager->GetTamanyoLadoZona())].Tipo == ETipoCasilla::Puente){
			
		FVector2D VectorYX2 = MundoManager->GetZonaManager()->ObtenerPosicionConIndexCasillasOZonas(MundoManager->GetMapaZonas().Find(Zona), MundoManager->GetTamanyoLadoMundo());
			VectorYX2 = YX ? FVector2D(VectorYX2.X, VectorYX2.Y + Valor) : FVector2D(VectorYX2.X + Valor, VectorYX2.Y);
			
			if(MundoManager->ComprobarLimitesDeMatriz(VectorYX2, MundoManager->GetTamanyoLadoMundo())){
				
				AZona* ZonaAPasar = MundoManager->GetMapaZonas()[MundoManager->GetZonaManager()->ObtenerIndexConPosicionCasillasOZonas(VectorYX2, MundoManager->GetTamanyoLadoMundo())];
					
				for (const FDatosCasillas& item : ZonaAPasar->GetDatosCasillasPuentes()) {
							
					if (YX) {
						if(item.GridPosition == FVector2D(VectorYX1.X, ValorComprobacionContraria)){
							SetDatosADevolver(ZonaAPasar, item.GridPosition, true);
							return;
						}
					}
					else {
						if (item.GridPosition == FVector2D(ValorComprobacionContraria,VectorYX1.Y)) {
							SetDatosADevolver(ZonaAPasar, item.GridPosition, true);
							return;
						}
					}	
				}
			}else{
				SetDatosADevolver(nullptr, FVector2D(0.f, 0.f), false);
				return;
			}
		}else{
			SetDatosADevolver(nullptr, FVector2D(0.f, 0.f), false);
			return;
		}
		
	}else{
		SetDatosADevolver(Zona, VectorYX1, true);
		return;
	}
	
}

TArray<AEnemigo*> UEnemigoManager::GenerarEnemigosInicioPartida()
{
	
	TArray<TSubclassOf<AActor>> Clases = UClaseDeUtilidades::ObtenerTodasLasClasesHijas(AEnemigo::StaticClass());

	int Index = 0;
	TArray<AEnemigo*> EnemigosGenerados;
	uint8 MaxEnemigosPorTipo = 100;
	
	for (TSubclassOf<AActor> item : Clases)
	{
		
		while (EnemigosGenerados.Num() < MaxEnemigosPorTipo * Index + 1)
		{
			//SPAWN ENEMIGO
			AEnemigo* Enemigo = GetWorld()->SpawnActor<AEnemigo>(

				item,
				FTransform(FRotator::ZeroRotator, FVector(0, 0, -10000), FVector(1, 1, 1))

			);

			EnemigosGenerados.Add(Enemigo);
			//FALTA CAMBIAR ESTE METODO A GLOBAL
			DesActivarEnemigo(Enemigo,false);

		} 
		
	}

	return EnemigosGenerados;

}

void UEnemigoManager::DesActivarEnemigo(AEnemigo* Enemigo, bool Activar)
{

	if (Activar) {
	
		Enemigo->SetActorHiddenInGame(false);
		Enemigo->GetMesh()->Activate();
		Enemigo->GetCharacterMovement()->Activate();
		Enemigo->SetActorEnableCollision(true);
		Enemigo->SetActorTickEnabled(true);
		
		if (Cast<AEnemigo>(Enemigo)) {
		//Falta IAMANAGER
		//Cast<IAManager>(Enemigo->GetOwner())
		//	IAManager->Avanzar();
		}


	}
	else {

		Enemigo->SetActorTickEnabled(false);
		Enemigo->SetActorEnableCollision(false);
		Enemigo->GetCharacterMovement()->Deactivate();
		Enemigo->GetMesh()->Deactivate();
		Enemigo->SetActorHiddenInGame(true);
	}
}

void UEnemigoManager::PosicionarEnemigo(TArray<FDatosCasillas> Ruta, FDatosCasillas DatosCasilla, AZona* Zona)
{
	
	//Habria que filtar aqui en caso de tipos de enemigo

	int Index = 0;
	AEnemigo* Enemigo;

	do{
	Enemigo = EnemigosGestionables[Index];
	
	Index++;

	} while (Enemigo->IsHidden() == true);


	FVector2D Posicion = DatosCasilla.GridPosition * MundoManager->GetTamanyoCasilla() + FVector2D(Zona->GetActorLocation().Y, Zona->GetActorLocation().X);

	Enemigo->SetActorLocation(FVector(Posicion.Y,Posicion.X,200));
	Enemigo->SetRuta(Ruta);
	Enemigo->SetVida(Enemigo->GetVidaMax());
	DesActivarEnemigo(Enemigo,true);
}
