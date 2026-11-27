// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/HUDController.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AHUDController::AHUDController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHUDController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass) {

		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (HUDWidget) {

			HUDWidget->AddToViewport();
		}

	}


}

void AHUDController::ActualizarEnergia(float Energia, float EnergiaMax)
{
	ActualizarBarraDeEnergia(Energia / EnergiaMax);
}





