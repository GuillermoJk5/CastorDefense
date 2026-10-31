// Fill out your copyright notice in the Description page of Project Settings.


#include "Mapa/Zonas/Zona.h"
#include "TiposDeDatosComunes/Enum/ETipoCasillaMapa.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
AZona::AZona()
	:Interseccion(false)
	,DatosCasillas()
	,DatosCasillasPuentes()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZona::BeginPlay()
{
	Super::BeginPlay();
	
	auto GetGroupFromCompName = [](const UHierarchicalInstancedStaticMeshComponent* InComp) -> ETipoCasilla
	{
		if (!InComp) return ETipoCasilla::NaN;
 
		const FString CompName = InComp->GetName();
		const UEnum* EnumPtr = StaticEnum<ETipoCasilla>();
 
		for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
		{
			FString InternalName = EnumPtr->GetNameStringByIndex(i); 
        
			if (CompName.Contains(InternalName, ESearchCase::IgnoreCase))
			{
				return (ETipoCasilla)EnumPtr->GetValueByIndex(i);
			}
		}

		
 
		return ETipoCasilla::NaN;
	};

	TInlineComponentArray<UHierarchicalInstancedStaticMeshComponent*> HISMComponents(this);

	for (UHierarchicalInstancedStaticMeshComponent* Hi : HISMComponents)
	{
		ETipoCasilla Group = GetGroupFromCompName(Hi);

		if (Group != ETipoCasilla::NaN)
		{
			HI.Add(Group, Hi);
		}
	}
}

// Called every frame
void AZona::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AZona::GetInterseccion()
{
	return this->Interseccion;
}

void AZona::SetInterseccion(bool InInterseccion)
{
	this->Interseccion = InInterseccion;
}

TArray<FDatosCasillas>& AZona::GetDatosCasillas()
{
	return this->DatosCasillas;
}

void AZona::SetDatosCasillas(TArray<FDatosCasillas> InDatosCasillas)
{
	this->DatosCasillas = InDatosCasillas;
}

TArray<FDatosCasillas>& AZona::GetDatosCasillasPuentes()
{
	return this->DatosCasillasPuentes;
}

void AZona::SetDatosCasillasPuentes(TArray<FDatosCasillas> InDatosCasillas)
{
	this->DatosCasillas = InDatosCasillas;
}

TMap<ETipoCasilla, UHierarchicalInstancedStaticMeshComponent*> AZona::GetHI()
{
	return this->HI;
}

void AZona::SetHI(TMap<ETipoCasilla, UHierarchicalInstancedStaticMeshComponent*> InHI)
{
	this->HI = InHI;
}
