// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"
#include "ObjetoColocableInterface.h"

#include "ObjetoColocable.generated.h"

class ANucleo;

UCLASS()
class CASTORDEFENSE_API AObjetoColocable : public APawn, public IObjetoColocableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AObjetoColocable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	float CosteDeEnergia;

	UPROPERTY()
	ANucleo* Nucleo;

	UPROPERTY()
	bool Activada;

	UPROPERTY()
	ETipoCasilla CasillaValida;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Category = "Getter")
	ANucleo* GetNucleo();

	UFUNCTION(Category = "Setter")
	void SetNucleo(ANucleo* InNucleo);

	UFUNCTION(Category = "Getter")
	float GetCosteDeEnergia();

	UFUNCTION(Category = "Setter")
	void SetCosteDeEnergia(float InCosteDEnergia);

	UFUNCTION(Category = "Getter")
	bool GetActivada();

	UFUNCTION(Category = "Setter")
	void SetActivada(bool InActivada);

	UFUNCTION(Category = "Getter")
	ETipoCasilla GetCasillaValida();

	UFUNCTION(Category = "Setter")
	void SetCasillaValida(ETipoCasilla InCasillaValida);

	//Valorar
	UFUNCTION()
	void DesActivar();

	UFUNCTION()
	void CalcularEnergiaYActuvar();

};
