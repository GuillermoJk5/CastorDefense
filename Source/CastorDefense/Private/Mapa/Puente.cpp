// Fill out your copyright notice in the Description page of Project Settings.


#include "Mapa/Puente.h"
#include "Mapa/Zonas/Zona.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APuente::APuente()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	RootComponent = SceneComponent;

	MeshPuente = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puente"));
	MeshPuente->SetupAttachment(SceneComponent);

}

// Called when the game starts or when spawned
void APuente::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuente::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AZona* APuente::GetConectadoCon()
{
	return this->ConectadoCon;
}

void APuente::SetConectadoCon(AZona* InConectadoCon)
{
	this->ConectadoCon = InConectadoCon;
}

bool APuente::GetBajado()
{
	return this->Bajado;
}

void APuente::SetBajado(bool InBajado)
{
	this->Bajado = InBajado;
}

void APuente::BajarPuente()
{
	
	MeshPuente->SetRelativeRotation(MeshPuente->GetRelativeRotation() + FRotator(90.f, 0.f, 0.f));
	this->SetBajado(true);
}

