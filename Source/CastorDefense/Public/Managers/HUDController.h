// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDController.generated.h"

class UUserWidget;

UCLASS()
class CASTORDEFENSE_API AHUDController : public AHUD
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHUDController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Widget que queremos crear
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	//Instancia del Widget
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidget;


public:	

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ActualizarEnergia(float Energia, float EnergiaMax);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ActualizarBarraDeEnergia(float EnergiaActualizada);

};
