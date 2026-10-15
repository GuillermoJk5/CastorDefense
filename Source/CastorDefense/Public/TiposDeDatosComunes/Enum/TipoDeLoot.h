
#pragma once

#include "CoreMinimal.h"
#include "TipoDeLoot.generated.h"

UENUM(BlueprintType)
enum class ETipoDeLoot : uint8
{
	Escamas UMETA(DisplayName = "Escamas")
	, Dientes UMETA(DisplayName = "Dientes")
};
