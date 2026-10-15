// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemigo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemigoManager.h"
#include "AIEnemigoController.h"
#include "ClaseDeUtilidades.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "MundoManager.h"
#include "Loot.h"


// Sets default values
AEnemigo::AEnemigo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemigo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemigo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemigo::GetVida()
{
	return this->Vida;
}

float AEnemigo::GetVidaMax()
{
	return  this->VidaMax;
}

float AEnemigo::GetDanyo()
{
	return this->Danyo;
}

int AEnemigo::GetIndex()
{
	return  this->Index;
}

TArray<FDatosCasillas> AEnemigo::GetRuta()
{
	return  this->Ruta;
}

void AEnemigo::SetVida(float InVida)
{
	this->Vida = InVida;
}

void AEnemigo::SetVidaMax(float InVidaMax)
{
	this->VidaMax = InVidaMax;
}

void AEnemigo::SetDanyo(float InDanyo)
{
	this->Danyo = InDanyo;
}

void AEnemigo::SetIndex(int InIndex)
{
	this->Index = InIndex;
}

void AEnemigo::SetRuta(TArray<FDatosCasillas> InRuta)
{
	this->Ruta = InRuta;
}

void AEnemigo::SetLoot(TArray<ETipoDeLoot> InLoot)
{
	this->Loot = InLoot;
}

TArray<ETipoDeLoot> AEnemigo::GetLoot()
{
	return this->Loot;
}

FVector AEnemigo :: PredecirMovimiento(float TiempoOdjetivo){

	AAIEnemigoController* AI = Cast<AAIEnemigoController>(this->GetOwner());
	if (AI) {

		FVector PosicionActual = this->GetActorLocation();

		for (int i = AI->GetIndex(); i <= AI->GetCaminoRuta2().Num() - 2 ; i++) {
	
			FVector2D PosicionCasilla = (FVector2D(AI->GetCaminoRuta2()[i + 1].GridPosition.Y, AI->GetCaminoRuta2()[i + 1].GridPosition.X) * AI->GetMundoManager()->GetTamanyoCasilla());
			FVector CoordenadaMatriz = AI->GetCaminoRuta2()[i + 1].HIDuenyo->GetOwner()->GetActorLocation();

			FVector CoordenadaFinal(
				CoordenadaMatriz.X + PosicionCasilla.X, 
				CoordenadaMatriz.Y + PosicionCasilla.Y,
				200);

			if (TiempoOdjetivo <= FVector::Dist(PosicionActual, CoordenadaFinal) / GetCharacterMovement()->MaxWalkSpeed) {
			
				return FVector(PosicionActual + FVector2D(FVector2D(CoordenadaFinal) - FVector2D(PosicionActual)).Normalize() * GetCharacterMovement()->MaxWalkSpeed * TiempoOdjetivo);
			
			}
			else {

				TiempoOdjetivo = TiempoOdjetivo - FVector::Dist(PosicionActual, CoordenadaFinal) / GetCharacterMovement()->MaxWalkSpeed;

				if (i == AI->GetCaminoRuta2().Num() - 2) {
				
					return CoordenadaFinal;
				
				}
				else {
				
					PosicionActual = CoordenadaFinal;
				}
			}
		}
	}

	return FVector();
}

void AEnemigo::RecibirHerida(float Herida)
{
	Vida = Vida - Herida;
	if (Vida <= 0) {
	
		//SPAWN LOOT
			for(ETipoDeLoot item : this->Loot){
		
			ALoot* NLoot = GetWorld()->SpawnActorDeferred<ALoot>(
				ALoot::StaticClass(),
				FTransform(),
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
			if (NLoot) {
				NLoot->SetTipoDeLoot(item);
				NLoot->FinishSpawning(FTransform(FRotator::ZeroRotator, this->GetActorLocation()));
			}
		}

		Destruir();
		
	}
}


void AEnemigo::Destruir()
{
	UClaseDeUtilidades::ActivarDesactivarActor(this, false);

	this->SetActorLocation(
		FVector(0, 0, -10000),
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}

