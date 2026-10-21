// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TiposDeDatosComunes/Enum/TipoDeLoot.h"
#include "Loot.generated.h"

UCLASS()
class CASTORDEFENSE_API ALoot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoot();

	UFUNCTION(Category = "Getter")
	ETipoDeLoot GetTipoDeLoot();

	UFUNCTION(Category = "Setter")
	void SetTipoDeLoot(ETipoDeLoot InTipoDeLoot);

	UFUNCTION(Category = "Getter")
	uint8 GetCantidad();

	UFUNCTION(Category = "Setter")
	void SetCantidad(uint8 InCantidad);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	ETipoDeLoot TipoLoot;

	UPROPERTY()
	uint8 Cantidad;
};
