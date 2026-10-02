// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//Enum ETipoCasillas
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"	

//Struct
#include "TiposDeDatosComunes/Struct/FDatosCasillas.h"
#include "Zona.generated.h"

class APuente;

UCLASS()
class CASTORDEFENSE_API AZona : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZona();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Getter")
	bool GetInterseccion();

	UFUNCTION(Category = "Setter")
	void SetInterseccion(bool InInterseccion);

	UFUNCTION(Category = "Getter")
	TArray<FDatosCasillas> GetDatosCasillas();

	UFUNCTION(Category = "Setter")
	void SetDatosCasillas(TArray<FDatosCasillas> InInterseccion);

	UFUNCTION(Category = "Getter")
	TArray<FDatosCasillas> GetDatosCasillasPuentes();

	UFUNCTION(Category = "Setter")
	void SetDatosCasillasPuentes(TArray<FDatosCasillas> InInterseccion);

private:

	UPROPERTY(EditAnywhere)
	bool Interseccion = false;

	UPROPERTY(EditAnywhere)
	TArray<FDatosCasillas> DatosCasillas;

	UPROPERTY(EditAnywhere)
	TArray<FDatosCasillas> DatosCasillasPuentes;

};
