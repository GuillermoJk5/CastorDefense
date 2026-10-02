

#pragma once

//Valorar llevar el enum y/o Struct que se usen para varias variables a un sitio externo (archivo .h sin cpp, e incluirlo en todas las que usen dichas variables)
//#include "MundoManager.h"

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

//Enum TipoCasillas
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"	
#include "ZonaManager.generated.h"


class UPinguinoManager;
class UMundoManager;
class ANucleo;
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

	//FALTA HIX
	UFUNCTION()
	int InstanciarCasilla(/*¿StaticMesh?* HIX, */FVector2D PosicionCasilla);

	UFUNCTION()
	FRotator RotarPuentes(FVector2D PosicionCasilla);

	UFUNCTION()
	AZona* GenerarZona(FVector2D CoordenadasZona);

	//FALTA ZonaInicial
	UFUNCTION()
	AZona* GenerarZonaInicial(FVector2D CoordenadasZona);

	//FALTA ZonaPortal
	UFUNCTION()
	AZona* GenerarZonaPortal(FVector2D CoordenadasZona);

	UFUNCTION()
	void HabilitarZonas(AZona* Zona);

	UFUNCTION()
	FVector2D CalcularPosicionPuenteZonaPrevia(AZona* Zona, FVector2D CoordenadasZonaNueva, FVector2D PosicionZonaPrevia);

	UFUNCTION()
	void ComprobarSiHayCasillasAdyacentes(FVector2D PosicionCasillas, TArray<ETipoCasilla> MapaSimple, TArray<FVector2D>& PCA, TArray<ETipoCasilla>& CasillasExistentesSimplificado);

	UFUNCTION()
	int ObtenerIndexConPosicionCasillasOZonas(FVector2D Posicion, int TamanyoLadoArray);

	UFUNCTION()
	FVector2D ObtenerPosicionConIndexCasillasOZonas(int Index, int TamanyoLadoArray);

	UFUNCTION()
	void ComprobarGeneracionDePuenteEsquinado(TArray<FVector2D> Puentes, FVector2D PuenteNuevo, bool& Correcto, FVector2D& MovimientoPosible);

	//FALTA Objetivo, HIX
	UFUNCTION()
	void PintarMapaDeCasillas(TArray<ETipoCasilla> MapaSimplificado, AZona* Zona);

	UFUNCTION()
	void BuscarZonasDiagonales(FVector2D PosicionNuevoPuente, FVector2D PosicionNuevaZona, bool& Diagonales, FVector2D& PosicionZonaDiagonalANueva);

	UFUNCTION()
	void AsociarPuentesConZonas(TArray<AZona*> MapaZonas);

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
	UFUNCTION()
	ETipoCasilla ComprobacionesDeCaminoYPuenteEnZonasPortal(int opcion, int X, int Y);

	UFUNCTION()
	bool ComprobarSiHayInterseccion(FVector2D CoordenadasZona, int CantidadFinCilos);

private:

	UPROPERTY(EditAnywhere, Category = "Migracion");
	UMundoManager* MundoManager;

	UPROPERTY(EditAnywhere, Category = "Migracion");
	UPinguinoManager* PinguinoManager;

};
