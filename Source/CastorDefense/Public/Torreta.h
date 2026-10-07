// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjetoColocable.h"

#include "ObjetoColocableInterface.h"

#include "Torreta.generated.h"

class AEnemigo;
class UCargadorTorretas;

/**
 * 
 */
UCLASS()
class CASTORDEFENSE_API ATorreta : public AObjetoColocable

{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Getter")
	AEnemigo* GetObjetivo();

	UFUNCTION(Category = "Setter")
	void SetObjetivo(AEnemigo* InObjetivo);

	UFUNCTION(Category = "Getter")
	TArray<AEnemigo*> GetEnRango();

	UFUNCTION(Category = "Setter")
	void SetEnRango(TArray<AEnemigo*> InEnRango);

	UFUNCTION(Category = "Getter")
	UCargadorTorretas* GetCargadorTorreta();

	UFUNCTION(Category = "Setter")
	void SetCargadorTorreta(UCargadorTorretas* InAC_CargadorTorreta);

	UFUNCTION(Category = "Getter")
	USphereComponent* GetAreaDeteccion();

	UFUNCTION(Category = "Setter")
	void SetAreaDeteccion(USphereComponent* InAreaDeteccion);

	UFUNCTION(Category = "Getter")
	USphereComponent* GetAreaRango();

	UFUNCTION(Category = "Setter")
	void SetAreaRango(USphereComponent* InAreaRango);

	UFUNCTION(Category = "Getter")
	FTimerHandle GetTiempoProximoDisparo();

	UFUNCTION(Category = "Setter")
	void SetTiempoProximoDisparo(FTimerHandle InTiempoProximoDisparo);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ObjetivoSaldraDeArea();

	virtual void Recargar(/*FALTA Caja de municion*/) override;

	UFUNCTION()
	void Disparar();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaDeteccion;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaRango;

	UPROPERTY()
	AEnemigo* Objetivo;

	UPROPERTY()
	TArray<AEnemigo*> EnRango;

	UPROPERTY()
	UCargadorTorretas* AC_CargadorTorreta;

	FTimerHandle TiempoProximoDisparo;
};
