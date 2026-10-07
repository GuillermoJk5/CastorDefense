// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TiposDeDatosComunes/Enum/TipoMunicion.h"	
#include "CargadorTorretas.generated.h"

class ABala;
class ATorreta;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CASTORDEFENSE_API UCargadorTorretas : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCargadorTorretas();

	UFUNCTION(Category = "Getter")
	int GetCapacidadMax();

	UFUNCTION(Category = "Setter")
	void SetCapacidadMax(int InCapacidadMaxima);

	UFUNCTION(Category = "Getter")
	int GetMunicion();

	UFUNCTION(Category = "Setter")
	void SetMunicion(int InMunicion);

	UFUNCTION(Category = "Getter")
	TArray<ABala*> GetBalas();

	UFUNCTION(Category = "Setter")
	void SetBalas(TArray<ABala*> InBalas);

	UFUNCTION(Category = "Getter")
	int GetIndexBalas();

	UFUNCTION(Category = "Setter")
	void SetIndexBalas(int InIndexBalas);

	UFUNCTION()
	void Disparo();

	UFUNCTION()
	bool DisparoUtil();

	UFUNCTION()
	void DispararBala();

	UFUNCTION()
	void Recargar(TArray<ACajaMunicion*> Municion);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY()
	int CapacidadMax = 10;

	UPROPERTY()
	int Municion = 20;
		
	UPROPERTY()
	TArray<ABala*> Balas;

	UPROPERTY()
	int IndexBalas = 0;

	UPROPERTY()
	ETipoMunicion TipoMunicionPermitida = ETipoMunicion::Balas;
};
