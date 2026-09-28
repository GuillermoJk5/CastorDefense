

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ZonaManager.generated.h"

class UPinguinoManager;
/**
 * Clase que se encarga de gestionar las zonas del mundo
 */
UCLASS(Blueprintable)
class CASTORDEFENSE_API UZonaManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, Category = "Migracion");
	TSubclassOf<AActor> mundoManager;

	UPROPERTY(EditAnywhere, Category = "Migracion");
	UPinguinoManager* pinguinoManager;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "MiSistema")
	AActor* GenerarZona();
private:

	void AsignarObjetos();

	FTimerHandle TimerHandle_AssignSubsystem;
};
