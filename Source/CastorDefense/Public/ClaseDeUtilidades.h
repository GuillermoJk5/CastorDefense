// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ClaseDeUtilidades.generated.h"

/**
 * Metodo que devuelve los hijos de una clase padre otorgada, en caso de no existir ninguno devuelve dicha clase padre
 */
UCLASS()
class CASTORDEFENSE_API UClaseDeUtilidades : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable, Category = "Class Utilities")
    static TArray<TSubclassOf<AActor>> ObtenerTodasLasClasesHijas(TSubclassOf<AActor> ClasePadre);
};
