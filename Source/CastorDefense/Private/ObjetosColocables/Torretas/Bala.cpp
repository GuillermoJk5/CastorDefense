// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjetosColocables/Torretas/Bala.h"
#include "ObjetosColocables/Torretas/Torreta.h"
#include "Enemigo/Enemigo.h"
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
	ProjectileMovement->bInitialVelocityInLocalSpace = false;
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

void ABala::ActivarBala(FVector Posicion, FRotator Rotacion, FVector PosicionFinal)
{
	this->SetActorLocationAndRotation(Posicion,Rotacion, false, nullptr, ETeleportType::TeleportPhysics);
	
	UClaseDeUtilidades::ActivarDesactivarActor(this, true);

	ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->MaxSpeed;

	//Ver Prediccion en el mapa
	DrawDebugSphere(GetWorld(), PosicionFinal, 30.f, 12, FColor::Red, false, 2.f);
	DrawDebugLine(GetWorld(), Posicion, Posicion + ProjectileMovement->Velocity.GetSafeNormal() * 1000.f, FColor::Green, false, 2.f);

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

		UClaseDeUtilidades::ActivarDesactivarActor(this, false);

	}

}

