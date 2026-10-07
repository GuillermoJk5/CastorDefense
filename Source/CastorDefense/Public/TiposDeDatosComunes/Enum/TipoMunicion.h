// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TipoMunicion.generated.h"

UENUM(BlueprintType)
enum class ETipoMunicion : uint8
{
	Balas  UMETA(DisplayName = "Balas")
	, Energia UMETA(DisplayName = "Energia")
};
