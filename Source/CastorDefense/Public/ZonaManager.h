

#pragma once

//Valorar llevar el enum y/o Struct que se usen para varias variables a un sitio externo (archivo .h sin cpp, e incluirlo en todas las que usen dichas variables)
#include "MundoManager.h"

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ZonaManager.generated.h"


class UPinguinoManager;
class UMundoManager;
class AZona;

/**
 * Clase que se encarga de gestionar las zonas del mundo
 */
UCLASS(Blueprintable)
class CASTORDEFENSE_API UZonaManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:	


	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION()
	UMundoManager* GetMundoManager();

	UFUNCTION()
	void SetMundoManager(UMundoManager* InMundoManager);

	UFUNCTION()
	UPinguinoManager* GetPinguinoManager();

	UFUNCTION()
	void SetPinguinoManager(UPinguinoManager* InPinguinoManager);

	//FALTA
	UFUNCTION()
	int InstanciarCasilla(/*¿StaticMesh?* HIX, */FVector2D PosicionCasilla);

	//FALTA PUENTE
	UFUNCTION()
	void RotarPuentes(/*UPuente* Puente, */FVector2D PosicionCasilla);

	//FALTA COMPROBARZONASADYACENTES
	//FALTA PINGUINOBORRACHO
	UFUNCTION()
	AZona* GenerarZona(FVector2D CoordenadasZona);

	UFUNCTION()
	AZona* GenerarZonaInicial(FVector2D CoordenadasZona);

	UFUNCTION()
	AZona* GenerarZonaPortal(FVector2D CoordenadasZona);

	//FALTA ZONA
	//UFUNCTION()
	//void HabilitarZonas(UZona* Zona);

	UFUNCTION()
	FVector2D CalcularPosicionPuenteZonaPrevia(AZona* Zona, FVector2D CoordenadasZonaNueva, FVector2D PosicionZonaPrevia);

	UFUNCTION()
	void ComprobarSiHayCasillasAdyacentes(FVector2D PosicionCasillas, TArray<ETipoCasilla> MapaSimple, FVector2D& PCA, TArray<ETipoCasilla>& CasillasExistentesSimplificado);

	UFUNCTION()
	int ObtenerIndexConPosicionCasillasOZonas(FVector2D Posicion, int TamanyoLadoArray);

	UFUNCTION()
	FVector2D ObtenerPosicionConIndexCasillasOZonas(int Index, int TamanyoLadoArray);

	UFUNCTION()
	void ComprobarGeneracionDePuenteEsquinado(TArray<FVector2D> Puentes, FVector2D PuenteNuevo, bool& Correcto, FVector2D& MovimientoPosible);

	UFUNCTION()
	void PintarMapaDeCasillas(TArray<ETipoCasilla> MapaSimplificado/*, UZona Zona*/);

	UFUNCTION()
	void BuscarZonasDiagonales(FVector2D PosicionNuevoPuente, FVector2D PosicionNuevaZona, bool& Diagonales, FVector2D& PosicionZonaDiagonalANueva);

	UFUNCTION()
	void AsociarPuentesConZonas(/*TArray<UZona> Zona*/);

	UFUNCTION()
	FVector2D VectorDeLadoZona();


	FORCEINLINE bool ComprobacionesPuentesEnZonaInicial(int X, int Y, int TamanyoLadoZona) {
		return(
			(X == 0 && Y == 5) 
			|| (X == TamanyoLadoZona && Y == 5 ) 
			|| (Y == 0 && X == 5) 
			|| (Y == TamanyoLadoZona && X == 5 )
		);
	}

	FORCEINLINE ETipoCasilla ComprobacionesDeCaminoYPuenteEnZonasPortal(int opcion, int X, int Y) {
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

	FORCEINLINE bool ComprobarSiHayInterseccion(FVector2D CoordenadasZona, int CantidadFinCilos) {
	
		FVector2D Vector = CoordenadasZona / VectorDeLadoZona();

		if (MundoManager->GetTamanyoLadoMundo() - 1 == Vector.X 
			&& Vector.X == 0
			&& MundoManager->GetTamanyoLadoMundo() - 1 == Vector.Y
			&& Vector.Y == 0) {
		
			return true;
		}
		else if(CantidadFinCilos != 1) {
		
			return true;
		}
		else {
			return false;
		}
	}
private:

	UPROPERTY(EditAnywhere, Category = "Migracion");
	UMundoManager* MundoManager;

	UPROPERTY(EditAnywhere, Category = "Migracion");
	UPinguinoManager* PinguinoManager;

};
