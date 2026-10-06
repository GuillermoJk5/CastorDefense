// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TiposDeDatosComunes/Struct/FDatosCasillas.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIEnemigoController.generated.h"

class UMundoManager;
/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API AAIEnemigoController : public AAIController
{
	GENERATED_BODY()

public:

	

	UFUNCTION()
	void SetMundoManager(UMundoManager* InMundoManager);

	UFUNCTION()
	void Avanzar();

	UFUNCTION()
	void IrA(FVector2D Coordenadas);

protected:

UFUNCTION()
	virtual void OnMoveCompleted(
		FAIRequestID RequestID,
		EPathFollowingResult::Type Result
	) override;

private:

	UPROPERTY()
	UMundoManager* MundoManager;

	UPROPERTY()
	int Index;

	UPROPERTY()
	TArray<FDatosCasillas> CaminoRuta2;


};


