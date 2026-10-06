// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjetoColocable.h"
#include "Generador.generated.h"

class UAC_GeneradorMunicion;

/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API AGenerador : public AObjetoColocable
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
