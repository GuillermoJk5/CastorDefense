// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mapa/Zonas/Zona.h"
#include "ZonaPortal.generated.h"

/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API AZonaPortal : public AZona
{
	GENERATED_BODY()
	
public:

	AZonaPortal();

	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Casillas", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> HI_SpawnEnemigo;
};
