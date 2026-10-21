// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Municion/CajaMunicion.h"
#include "Nucleo.generated.h"


UCLASS()
class CASTORDEFENSE_API ANucleo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANucleo();

	UFUNCTION(Category = "Getter")
	float GetVida();

	UFUNCTION(Category = "Setter")
	void SetVida(float InVida);

	UFUNCTION(Category = "Getter")
	float GetEnergiaMax();

	UFUNCTION(Category = "Setter")
	void SetEnergiaMax(float InEnergiaMax);

	UFUNCTION(Category = "Getter")
	float GetEnergiaRestante();

	UFUNCTION(Category = "Setter")
	void SetEnergiaRestante(float InEnergiaRestante);

	UFUNCTION()
	TArray<ACajaMunicion*> GetMunicionCreada();

	UFUNCTION()
	void SetMunicionCreada(TArray<ACajaMunicion*> inCajaMunicion);

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UStaticMeshComponent* CollisionMesh;

	UFUNCTION()
	void OnHandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void PerderVida(float Danyo);

	UFUNCTION()
	bool ActualizarEnergia(float CosteDeEnergia);

	UFUNCTION()
	void GenerarMunicion(ETipoMunicion TipoMunicion, ETamanyoMunicion TamanyoMunicion, AActor* Generador/*Falta Icono??*/);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	float Vida;

	UPROPERTY()
	float EnergiaMax;

	UPROPERTY()
	float EnergiaRestante;

	UPROPERTY()
	TArray<ACajaMunicion*> MunicionCreada;

};
