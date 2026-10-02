// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UStaticMeshComponent* CollisionMesh;

	UFUNCTION()
	void OnHandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void PerderVida(float Danyo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	float Vida;

	float EnergiaMax;

	float EnergiaRestante;
};
