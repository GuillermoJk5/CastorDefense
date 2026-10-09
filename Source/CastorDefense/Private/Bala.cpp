// Fill out your copyright notice in the Description page of Project Settings.


#include "Bala.h"
#include "Torreta.h"
#include "Enemigo.h"
#include "ClaseDeUtilidades.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
ABala::ABala()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
        TEXT("ProjectileMovement")
    );

    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 1000.0f;
    ProjectileMovement->bRotationFollowsVelocity = false;
}

ATorreta* ABala::GetTorreta()
{
	return this->Torreta;
}

void ABala::SetTorreta(ATorreta* InTorreta)
{
	this->Torreta = InTorreta;
}

float ABala::GetDanyo()
{
	return this->Danyo;
}

void ABala::SetDanyo(float InDanyo)
{
	this->Danyo = InDanyo;
}

void ABala::ActivarBala(FVector Posicion, FRotator Rotacion)
{
	this->SetActorLocationAndRotation(Posicion,Rotacion,false,nullptr,ETeleportType ::TeleportPhysics );
	//LLAMAR METDODO GLOBAL ACTIVAR ACTOR

	ProjectileMovement->Velocity = FVector(ProjectileMovement->MaxSpeed, 0.f, 0.f);
	ProjectileMovement->Activate();

	
}

void ABala::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemigo* Enemigo = Cast<AEnemigo>(OtherActor);
	
	if (Enemigo) {

		Enemigo->RecibirHerida(Danyo);
		
		UClaseDeUtilidades :: ActivarDesactivarActor(this, false);
	
	}
			
}

// Called when the game starts or when spawned
void ABala::BeginPlay()
{
	Super::BeginPlay();	
	UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABala::OnOverlapBegin);

}

// Called every frame
void ABala::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!this->IsHidden() && FVector::Dist(Torreta->GetActorLocation(), this->GetActorLocation()) > Torreta->GetAreaDeteccion()->GetScaledSphereRadius()) {

		//DESACTIVAR  ACTOR METODO GLOBAL


	}

}

