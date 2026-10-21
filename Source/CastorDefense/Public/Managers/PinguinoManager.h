// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

//Enum TipoCasillas
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"	
#include "PinguinoManager.generated.h"

class UZonaManager;
/**
 * Clase que se encarga de realizar el metodo del pingüino borracho para la generación de mapa
 */
UCLASS(Blueprintable)
class CASTORDEFENSE_API UPinguinoManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	//Getters y Setters
	UFUNCTION()
	UZonaManager* GetZonaManager();

	UFUNCTION()
	void SetZonaManager(UZonaManager* InZonaManager);

	UFUNCTION()
	bool PinguinoBorracho(TArray<FVector2D> PosicionesPuentes, TArray<ETipoCasilla> MapaRecursivo);

	UFUNCTION()
	TArray<FVector2D> CambiarProbabilidadMovimiento(FVector2D CasillaNueva, FVector2D CasillaActual, TArray<FVector2D> DireccionDinamica);

	UFUNCTION()
	FVector2D DireccionDelPinguino(TArray<FVector2D> Probabilidad, FVector2D PosicionActual);

	UFUNCTION()
	bool ComprobarTipoMaxCasillasAdyacentes(TArray<ETipoCasilla> DireccionDinamica, int MaximoDeCasillas, ETipoCasilla TipoDeCasilla);

	UFUNCTION()
	bool ComprobacionesPuenteConectado(TArray<ETipoCasilla> MapaSimple, FVector2D PosicionPuente);

	UFUNCTION()
	void ComprobarDestinoCamino(TArray<ETipoCasilla> CasillasAdyacentesExistentes, bool& Bool, bool& PuenteAdyacente);

	UFUNCTION()
	bool HaciaFinPinguino(TArray<FVector2D> PuentesRestantes, TArray<ETipoCasilla> MapaSimplificado, FVector2D PosicionAleatoriaNuevaCasilla);

	UFUNCTION()
	void HaciaCrearCasillaYContinuar(TArray<FVector2D> ProbMovimientoEstatica, FVector2D PosicionAleatoriaNuevaCasilla, TArray<ETipoCasilla> MapaAPintar);

	UFUNCTION()
	void BorrarYReiniciar();

	UFUNCTION()
	void CalcularProbabilidadesDelMovimiento(FVector2D PosicionAleatoriaNuevaCasilla, TArray<ETipoCasilla> MapaSimplificado);

	UFUNCTION()
	TArray<FVector2D> ObtenerTodosLosPuentesDeMapaAPintar(TArray<ETipoCasilla> MapaSimplificado);

	UFUNCTION()
	TArray<FVector2D> EliminarProbabilidadesMovimientosSiBordes(FVector2D PCI, TArray<FVector2D> Direcciones);

	FORCEINLINE ETipoCasilla ObtenerDestinoAdyacente(TArray<ETipoCasilla> CasillasAdyacentesExistentes) {
		if (CasillasAdyacentesExistentes.Contains(ETipoCasilla::CaminoConectado)) {
			return ETipoCasilla::CaminoConectado;
		}
		if (CasillasAdyacentesExistentes.Contains(ETipoCasilla::PuenteConectado)) {
			return ETipoCasilla::PuenteConectado;
		}
		if (CasillasAdyacentesExistentes.Contains(ETipoCasilla::Puente)) {
			return ETipoCasilla::Puente;
		}
		return ETipoCasilla::NaN;
	}

	FORCEINLINE FVector2D ObtenerPosicionDestino(TArray<ETipoCasilla> CasillasAdyacentesExistentes, ETipoCasilla Destino, TArray<FVector2D> PosicionesCasillasAdyacentesExistenes) {
		
		return PosicionesCasillasAdyacentesExistenes[CasillasAdyacentesExistentes.Find(Destino)];
	}

private:

	UPROPERTY()
	UZonaManager* ZonaManager;
};
