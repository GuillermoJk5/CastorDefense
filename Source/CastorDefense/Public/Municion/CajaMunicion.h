// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TiposDeDatosComunes/Enum/TipoMunicion.h"
#include "TiposDeDatosComunes/Enum/TamanyoMunicion.h"
#include "CajaMunicion.generated.h"

UCLASS()
class CASTORDEFENSE_API ACajaMunicion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACajaMunicion();

	UFUNCTION(Category = "Getter")
	ETipoMunicion GetTipoMunicion();

	UFUNCTION(Category = "Setter")
	void SetTipoMunicion(ETipoMunicion InTipoMunicion);

	UFUNCTION(Category = "Getter")
	ETamanyoMunicion GetTamanyoMunicion();

	UFUNCTION(Category = "Setter")
	void SetTamanyoMunicion(ETamanyoMunicion InTamanyoMunicion);

	UFUNCTION(Category = "Getter")
	UTexture* GetTextura();

	UFUNCTION(Category = "Setter")
	void SetTextura(UTexture* InTextura);

	//FALTA emparentar, ya que la funcionalidad es mas del blueprint que de c++
	//UFUNCTION(BlueprintImplementableEvent, Category = "Visual")
	//void GenerarTipoMunicion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	ETipoMunicion TipoMunicion;
	
	UPROPERTY()
	ETamanyoMunicion TamanyoMunicion;

	UPROPERTY()
	UTexture* Textura;
};
