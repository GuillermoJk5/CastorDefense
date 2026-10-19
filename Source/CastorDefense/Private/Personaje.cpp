// Fill out your copyright notice in the Description page of Project Settings.

#include "Personaje.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "PersonajeInput.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

#include "ObjetoColocable.h"
#include "Puente.h"
#include "Torreta.h"
#include "Zona.h"
#include "ZonaInicial.h"
#include "Loot.h"
#include "CajaMunicion.h"
#include "MundoManager.h"
#include "ZonaManager.h"


UMundoManager* APersonaje::GetMundoManager()
{
	return this->MundoManager;
}

TArray<ACajaMunicion*> APersonaje::GetInventarioCinturon()
{
	return this->InventarioCinturon;
}

float APersonaje::GetTimer()
{
	return this->Timer;
}

void APersonaje::SetTimer(float inTimer)
{
	this->Timer = inTimer;
}

void APersonaje::SetInventarioCinturon(TArray<ACajaMunicion*> inInventarioCinturon)
{
	this->InventarioCinturon = inInventarioCinturon;
}

void APersonaje::SetMundoManager(UMundoManager* inMundoManager)
{
	this->MundoManager = inMundoManager;
}

void APersonaje::Colocar(ETiposColocables Tipo)
{
	FDatosCasillas Casilla;
	FVector2D CoordenadasCasilla;
	MundoManager->ObtenerCasillaParaColocar(this->GetActorLocation(),Casilla, CoordenadasCasilla);

	if (!Casilla.Ocupada) {

		//FALTA
		//MEJORAR ESTA CHAPUZA
		switch (Tipo) {
		
		case ETiposColocables::Torreta:{

			ATorreta* Torrea = GetWorld()->SpawnActorDeferred<ATorreta>(
				ATorreta::StaticClass(),
				FTransform(FRotator::ZeroRotator, FVector(CoordenadasCasilla.X, CoordenadasCasilla.Y, 100)),
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
			if (Torrea) {
				if (Torrea->GetCasillaValida() == Casilla.Tipo) {

					Torrea->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(CoordenadasCasilla.X, CoordenadasCasilla.Y, 100)));

					int a = MundoManager->GetMapaZonas().Find(Cast<AZona>(Casilla.HIDuenyo->GetOwner()));
					int b = MundoManager->GetMapaZonas()[a]->GetDatosCasillas().Find(Casilla);

					MundoManager->GetMapaZonas()[a]->GetDatosCasillas()[b].Ocupada = true;

					Torrea->CalcularEnergiaYActuvar();
				}
			}

			break;
		}

		case ETiposColocables::Generador:{

			break;
		}

		}

	}

}

void APersonaje::ColocarTorreta()
{
	Colocar(ETiposColocables::Torreta);
}

void APersonaje::ColocarGenerador()
{
	Colocar(ETiposColocables::Generador);
}


void APersonaje::Interactuar(const FInputActionValue& Value)
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation;

	
	float Radius = 150.f;
	float HalfHeight = 100.f;
	FCollisionShape MyCapsule = FCollisionShape::MakeCapsule(Radius, HalfHeight);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Evita que la cápsula se choque con el ejecutor
	QueryParams.bTraceComplex = false; // Falso para usar colisión simple (mejor rendimiento)

	
	FHitResult HitResult;
	bool bHit = World->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,           // Rotación de la cápsula (usualmente la del Actor)
		ECC_Visibility,            // Canal de colisión
		MyCapsule,
		QueryParams
	);


#if !UE_BUILD_SHIPPING
	FColor DebugColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(World, StartLocation, HalfHeight, Radius, FQuat::Identity, DebugColor, false, 2.0f);
	DrawDebugLine(World, StartLocation, EndLocation, DebugColor, false, 2.0f);
#endif

	if (bHit) {
		AActor* ActorGolpeado = HitResult.GetActor();
		if (ActorGolpeado) {
			if (Cast<AObjetoColocable>(ActorGolpeado))
			{
				AObjetoColocable* ObjetoColocable = Cast<AObjetoColocable>(ActorGolpeado);
				ObjetoColocable->Interactuar();
			}
			else if (Cast<APuente>(ActorGolpeado))
			{
				APuente* Puente = Cast<APuente>(ActorGolpeado);
				if (!Puente->GetBajado())
				{
					MundoManager->GetZonaManager()->HabilitarZonas(Puente->GetConectadoCon());
					FTimerHandle TiempoDeEspera;
					GetWorldTimerManager().SetTimer(TiempoDeEspera, [this]()
					{

						//FALTA GESTOR OLEADAS
						//MundoManager->GetGestorOleadas()->JGO();
					}, 5.f, false);
				}
			}
		}
	}

}

void APersonaje::Movimiento(const FInputActionValue& Value)
{
	FVector2D VectorMovimiento = Value.Get<FVector2D>();
	AddMovementInput(FVector(VectorMovimiento.Y, VectorMovimiento.X, 0.f).GetSafeNormal(), 1.f);
}

void APersonaje::OnRecargarTriggered(const FInputActionInstance& Instance)
{
	UWorld* World = GetWorld();
	if (!World) return;

	
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation;

	
	float Radius = 150.f;
	float HalfHeight = 100.f;
	FCollisionShape MyCapsule = FCollisionShape::MakeCapsule(Radius, HalfHeight);

	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Evita que la cápsula se choque con el ejecutor
	QueryParams.bTraceComplex = false; // Falso para usar colisión simple (mejor rendimiento)

	
	FHitResult HitResult;
	bool bHit = World->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,           // Rotación de la cápsula (usualmente la del Actor)
		ECC_Visibility,            // Canal de colisión
		MyCapsule,
		QueryParams
	);

#if !UE_BUILD_SHIPPING
	FColor DebugColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(World, StartLocation, HalfHeight, Radius, FQuat::Identity, DebugColor, false, 2.0f);
	DrawDebugLine(World, StartLocation, EndLocation, DebugColor, false, 2.0f);
#endif

	if(bHit)
	{
		AActor* ActorGolpeado = HitResult.GetActor();
		if (ActorGolpeado) {
			if (Cast<ATorreta>(ActorGolpeado))
			{
				ATorreta* Ocupada = Cast<ATorreta>(ActorGolpeado);
				if (Instance.GetTriggeredTime() > Timer) {
					Ocupada->Recargar(InventarioCinturon);
					Timer = Instance.GetTriggeredTime() + 2.f;
				}
			}
			else
			{
				Timer = Instance.GetTriggeredTime() + 2.f;
			}
		}
	}
}

void APersonaje::OnRecargarCanceled()
{
	Timer = 0.f;
}

void APersonaje::OnRecargarCompleted()
{
	Timer = 0.f;
}

void APersonaje::DebugOleada(const FInputActionValue& Value)
{
	//FALTA GESTOR OLEADAS
	//MundoManager->GetGestorOleadas()->JGO();
}

// Sets default values
APersonaje::APersonaje()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APersonaje::ConsumirMunicion(ACajaMunicion* CajaMunicion)
{

		/*SET AC GENERADOR MUNICION -> MUNICION CREADA*/	InventarioCinturon[InventarioCinturon.Find(CajaMunicion)];
		InventarioCinturon[InventarioCinturon.Find(CajaMunicion)] = nullptr;
		
		//FALTA
		//INTERFAZ ACTUALIZAR INVENTARIO


}

void APersonaje::OnHandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACajaMunicion* CajaMuni = Cast<ACajaMunicion>(OtherActor);
	if (CajaMuni) {
		int index=0;
		for(ACajaMunicion* item : InventarioCinturon){
		
			if (item){
			
				InventarioCinturon.Insert(CajaMuni, index);
			}

			index++;
		}

		//FALTA
		//INTERFAZ ACTUALIZAR INVENTARIO

		CajaMuni->SetActorLocation(FVector(0, 0, -1000));

	}

	ALoot* NuevoLoot = Cast<ALoot>(OtherActor);
	if (NuevoLoot) {

		//FALTAA
		//AGREGAR LOGICA DE INVENTARIO DE LOOT
	
	}
}

// Called when the game starts or when spawned
void APersonaje::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if(PersonajeInput && PersonajeInput->DefaultMappingContext)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(PersonajeInput->DefaultMappingContext, 0);
			}
		}
	}

	AZonaInicial* ZonaInicial = Cast<AZonaInicial>(UGameplayStatics::GetActorOfClass(GetWorld(), AZonaInicial::StaticClass()));
	if(ZonaInicial){
		SetActorLocation(FVector(ZonaInicial->GetActorLocation().X, ZonaInicial->GetActorLocation().Y, ZonaInicial->GetActorLocation().Z + 250.f));
		InventarioCinturon.SetNum(4);
	}
	
}

// Called every frame
void APersonaje::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APersonaje::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC && PersonajeInput)
	{
		if (PersonajeInput->DAIAColocar) {
			EIC->BindAction(PersonajeInput->DAIAColocar, ETriggerEvent::Triggered, this, &APersonaje::ColocarTorreta);
		}

		if (PersonajeInput->DAIAColocarUNO) {
			EIC->BindAction(PersonajeInput->DAIAColocarUNO, ETriggerEvent::Triggered, this, &APersonaje::ColocarGenerador);
		}

		if (PersonajeInput->DAIAInteractuar) {
			EIC->BindAction(PersonajeInput->DAIAInteractuar, ETriggerEvent::Triggered, this, &APersonaje::Interactuar);
		}
		
		if (PersonajeInput->DAIAMovimiento) {
			EIC->BindAction(PersonajeInput->DAIAMovimiento, ETriggerEvent::Triggered, this, &APersonaje::Movimiento);
		}

		if (PersonajeInput->DAIARecargar) {
			EIC->BindAction(PersonajeInput->DAIARecargar, ETriggerEvent::Triggered, this, &APersonaje::OnRecargarTriggered);
			EIC->BindAction(PersonajeInput->DAIARecargar, ETriggerEvent::Canceled, this, &APersonaje::OnRecargarCanceled);
			EIC->BindAction(PersonajeInput->DAIARecargar, ETriggerEvent::Completed, this, &APersonaje::OnRecargarCompleted);
		}

		if (PersonajeInput->DAIADebugOleada) {
			EIC->BindAction(PersonajeInput->DAIADebugOleada, ETriggerEvent::Triggered, this, &APersonaje::DebugOleada);
		}

		
		/*if (PersonajeInput->DAIAColocar) {
		EIC->BindAction(PersonajeInput->DAIAColocar, ETriggerEvent::Triggered, this,
				UEnhancedInputComponent::FEnhancedInputHandlerValueSignature::CreateLambda([this](const FInputActionValue& Value)
					{
						Colocar(ETiposColocables::Torreta);
						UE_LOG(LogTemp, Warning, TEXT("¡C2665 solucionado con Lambda!"));
					}));
		
		}*/

	/*	if (PersonajeInput->DAIAColocarUNO) {
			EIC->BindAction(PersonajeInput->DAIAColocarUNO, ETriggerEvent::Triggered, this, [this](const FInputActionInstance& Instance) {
				Colocar( ETiposColocables::Generador);
				});
		}
		*/
		
	}

}

