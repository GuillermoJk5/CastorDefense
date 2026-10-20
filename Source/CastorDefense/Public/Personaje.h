// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TiposDeDatosComunes/Enum/TiposColocables.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Personaje.generated.h"


class UInputAction;
class UPersonajeInput;
class UMundoManager;
class ACajaMunicion;
struct FInputActionValue;
struct FInputActionInstance;



UCLASS()
class CASTORDEFENSE_API APersonaje : public ACharacter
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UPersonajeInput* PersonajeInput;

	UFUNCTION(Category="Getter")
	UMundoManager* GetMundoManager();

	UFUNCTION(Category = "Getter")
	TArray<ACajaMunicion*> GetInventarioCinturon();

	UFUNCTION(Category = "Getter")
	float GetTimer();

	UFUNCTION(Category = "Setter")
	void SetTimer(float inTimer);

	UFUNCTION(Category = "Setter")
	void SetInventarioCinturon(TArray<ACajaMunicion*> InInventarioCinturon);

	UFUNCTION(Category = "Setter")
	void SetMundoManager(UMundoManager* InMundoManager);

	// Handlers para las acciones
	void Colocar(ETiposColocables Tipo);
	void ColocarTorreta();
	void ColocarGenerador();
	void Interactuar(const FInputActionValue& Value);
	void Movimiento(const FInputActionValue& Value);
	void OnRecargarTriggered(const FInputActionInstance& Instance);
	void OnRecargarCanceled();
	void OnRecargarCompleted();
	void DebugOleada(const FInputActionValue& Value);

	

	// Sets default values for this character's properties
	APersonaje();

protected:

	UFUNCTION()
	void OnHandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ConsumirMunicion(ACajaMunicion* CajaMunicion);

private: 
	UPROPERTY()
	UMundoManager* MundoManager;

	UPROPERTY()
	TArray<ACajaMunicion*> InventarioCinturon;

	//FALTA INTEFAZ
	// 
	//UPROPERTY()
	//Interfaz* Interfaz;

	UPROPERTY()
	float Timer = 0.f;
};
