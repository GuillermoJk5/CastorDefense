// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemigoController.h"
#include "MundoManager.h"
#include "Enemigo.h"
#include "Algo/Reverse.h"
#include "Navigation/PathFollowingComponent.h"



int AAIEnemigoController::GetIndex()
{
	return this->Index;
}

TArray<FDatosCasillas> AAIEnemigoController::GetCaminoRuta2()
{
	return this->CaminoRuta2;
}

void AAIEnemigoController::SetIndex(int InIndex)
{
	this->Index = InIndex;
}

void AAIEnemigoController::SetCaminoRuta2(TArray<FDatosCasillas> InCaminoRuta2)
{
	this->CaminoRuta2 = InCaminoRuta2;
}

UMundoManager* AAIEnemigoController::GetMundoManager()
{
	return this->MundoManager;
}

void AAIEnemigoController::SetMundoManager(UMundoManager* InMundoManager)
{
	this->MundoManager = InMundoManager;
}

void AAIEnemigoController::Avanzar()
{
	AEnemigo* Poseido = Cast<AEnemigo>(GetPawn());

	if (Poseido) {

		CaminoRuta2 = Poseido->GetRuta();
		Algo::Reverse(CaminoRuta2);

		IrA(CaminoRuta2[Index].GridPosition * MundoManager->GetTamanyoCasilla() + FVector2D(CaminoRuta2[Index].HIDuenyo->GetOwner()->GetActorLocation().Y, CaminoRuta2[Index].HIDuenyo->GetOwner()->GetActorLocation().X));

	}
}

void AAIEnemigoController::IrA(FVector2D Coordenadas)
{

	//Cambio el MoveTo a MoveToLocation por q es menos complejo
	MoveToLocation(
		FVector(Coordenadas.Y,Coordenadas.X,200),
		10,		//Aceptable Radius
		false	//Overlap
	);
}


void AAIEnemigoController::OnMoveCompleted(FAIRequestID RequestID,EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result == EPathFollowingResult::Success)
	{
		Index++;
		IrA(CaminoRuta2[Index].GridPosition * MundoManager->GetTamanyoCasilla() + FVector2D(CaminoRuta2[Index].HIDuenyo->GetOwner()->GetActorLocation().Y, CaminoRuta2[Index].HIDuenyo->GetOwner()->GetActorLocation().X));

	}
}