
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

	UFUNCTION()
	static void ActivarDesactivarActor(AActor* Actor,bool Activar);
};
