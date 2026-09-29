// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Puente.generated.h"

class AZona;

UCLASS(Blueprintable)
class CASTORDEFENSE_API APuente : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuente();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Getter")
	AZona* GetZona();

	UFUNCTION(Category = "Setter")
	void SetZona(AZona* InZona);

	UFUNCTION(Category = "Getter")
	bool GetBajado();

	UFUNCTION(Category = "Setter")
	void SetBajado(bool InBajado);

private:

	UPROPERTY(EditAnywhere, Category = "Migracion")
	AZona* Zona;

	UPROPERTY(EditAnywhere, Category = "Migracion")
	bool Bajado;
};
