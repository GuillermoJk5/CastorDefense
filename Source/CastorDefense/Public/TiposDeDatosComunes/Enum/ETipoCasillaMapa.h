#pragma once

#include "CoreMinimal.h"
#include "ETipoCasillaMapa.generated.h"

UENUM(BlueprintType)
enum class ETipoCasilla : uint8
{
	NaN  UMETA(DisplayName = "NaN")
	, Terreno UMETA(DisplayName = "Terreno")
	, Camino  UMETA(DisplayName = "Camino")
	, CaminoConectado  UMETA(DisplayName = "CaminoConectado")
	, Puente  UMETA(DisplayName = "Puente")
	, PuenteConectado  UMETA(DisplayName = "PuenteConectado")
	, Objetivo  UMETA(DisplayName = "Objetivo")
	, SpawnEnemigo  UMETA(DisplayName = "SpawnEnemigo")
};