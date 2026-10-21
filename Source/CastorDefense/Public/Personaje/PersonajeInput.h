// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PersonajeInput.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API UPersonajeInput : public UDataAsset
{
	GENERATED_BODY()

	public:

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		UInputMappingContext* DefaultMappingContext;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* DAIAColocar;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* DAIAColocarUNO;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* DAIAInteractuar;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* DAIAMovimiento;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* DAIARecargar;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* DAIADebugOleada;
};
