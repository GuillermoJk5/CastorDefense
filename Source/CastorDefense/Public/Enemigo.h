// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TiposDeDatosComunes/Struct/FDatosCasillas.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemigo.generated.h"


UCLASS()
class CASTORDEFENSE_API AEnemigo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemigo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(Category = "Getter")
	float GetVida();

	UFUNCTION(Category = "Getter")
	float GetVidaMax();

	UFUNCTION(Category = "Getter")
	float GetDanyo();

	UFUNCTION(Category = "Getter")
	int GetIndex();

	UFUNCTION(Category = "Getter")
	TArray<FDatosCasillas> GetRuta();

	UFUNCTION(Category = "Setter")
	void SetVida(float InVida);

	UFUNCTION(Category = "Setter")
	void SetVidaMax(float InVidaMax);

	UFUNCTION(Category = "Setter")
	void SetDanyo(float InDanyo);

	UFUNCTION(Category = "Setter")
	void SetIndex(int InIndex);

	UFUNCTION(Category = "Setter")
	void SetRuta(TArray<FDatosCasillas> InRuta);

	UFUNCTION()
	FVector PredecirMovimiento(float TiempoOdjetivo);

	UFUNCTION()
	void RecibirHerida(float Herida);

	UFUNCTION()
	void Destruir();


private:

	UPROPERTY()
	float Vida;

	UPROPERTY()
	float VidaMax;

	UPROPERTY()
	int Index;

	UPROPERTY()
	TArray<FDatosCasillas> Ruta;

	//UPROPERTY()
	//TArray<ETipoLoot> Loot;

	UPROPERTY()
	float Danyo;






};
