// Fill out your copyright notice in the Description page of Project Settings.

#include "Personaje/Personaje.h"
#include "Personaje/PersonajeInput.h"
#include "ObjetosColocables/ObjetoColocable.h"
#include "ClaseDeUtilidades.h"
#include "Mapa/Nucleo.h"
#include "Mapa/Puente.h"
#include "ObjetosColocables/Torretas/Torreta.h"
#include "Mapa/Zonas/Zona.h"
#include "Mapa/Zonas/ZonaInicial.h"
#include "Enemigo/Loot/Loot.h"
#include "Municion/CajaMunicion.h"
#include "Managers/MundoManager.h"
#include "Managers/ZonaManager.h"
#include "Managers/OleadasManager.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"


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

			ATorreta* CajaMunicion = GetWorld()->SpawnActorDeferred<ATorreta>(
				ATorreta::StaticClass(),
				FTransform(FRotator::ZeroRotator, FVector(CoordenadasCasilla.X, CoordenadasCasilla.Y, 100)),
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
			if (CajaMunicion) {
				if (CajaMunicion->GetCasillaValida() == Casilla.Tipo) {

					CajaMunicion->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(CoordenadasCasilla.X, CoordenadasCasilla.Y, 100)));

					int a = MundoManager->GetMapaZonas().Find(Cast<AZona>(Casilla.HIDuenyo->GetOwner()));
					int b = MundoManager->GetMapaZonas()[a]->GetDatosCasillas().Find(Casilla);

					MundoManager->GetMapaZonas()[a]->GetDatosCasillas()[b].Ocupada = true;

					CajaMunicion->CalcularEnergiaYActuvar();
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
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	
	FHitResult HitResult;
	bool bHit = World->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,           
		ECC_Visibility,            
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
			if (AObjetoColocable* ObjetoColocable =Cast<AObjetoColocable>(ActorGolpeado))
			{ 
				ObjetoColocable->Interactuar();
			}
			else if (APuente* Puente = Cast<APuente>(ActorGolpeado))
			{
				if (!Puente->GetBajado())
				{
					MundoManager->GetZonaManager()->HabilitarZonas(Puente->GetConectadoCon());
					FTimerHandle TiempoDeEspera;
					GetWorldTimerManager().SetTimer(TiempoDeEspera, [this]()
					{
						MundoManager->GetOleadasManager()->JGO();
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
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false; 

	
	FHitResult HitResult;
	bool bHit = World->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,           
		ECC_Visibility,            
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
			if (ATorreta* Torreta =Cast<ATorreta>(ActorGolpeado))
			{
				
				if (Instance.GetTriggeredTime() > Timer) {
					Torreta->Recargar(InventarioCinturon);
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
	MundoManager->GetOleadasManager()->JGO();
}

// Sets default values
APersonaje::APersonaje()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APersonaje::ConsumirMunicion(ACajaMunicion* CajaMunicion)
{
	ANucleo* Nucleo = Cast<ANucleo>(UGameplayStatics::GetActorOfClass(GetWorld(), ANucleo::StaticClass()));
	
	Nucleo->GetMunicionCreada().Add(InventarioCinturon[InventarioCinturon.Find(CajaMunicion)]);

	InventarioCinturon[InventarioCinturon.Find(CajaMunicion)] = nullptr;
		
		//FALTA
		//INTERFAZ ACTUALIZAR INVENTARIO


}

void APersonaje::OnHandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (ACajaMunicion* CajaMuni = Cast<ACajaMunicion>(OtherActor)) {
		int index=0;
		for(ACajaMunicion* item : InventarioCinturon){
		
			if (item){
			
				InventarioCinturon.Insert(CajaMuni, index);
			}

			index++;
		}

		//FALTA
		//INTERFAZ ACTUALIZAR INVENTARIO

		UClaseDeUtilidades::ActivarDesactivarActor(CajaMuni, false);
		CajaMuni->SetActorLocation(FVector(0, 0, -1000));

	}else if (ALoot* NuevoLoot = Cast<ALoot>(OtherActor)) {

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

	;
	if(AZonaInicial* ZonaInicial = Cast<AZonaInicial>(UGameplayStatics::GetActorOfClass(GetWorld(), AZonaInicial::StaticClass()))){
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

