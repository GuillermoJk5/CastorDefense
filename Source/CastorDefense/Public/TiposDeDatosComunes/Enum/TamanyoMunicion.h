// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TamanyoMunicion.generated.h"

UENUM(BlueprintType)
enum class ETamanyoMunicion : uint8
{
	Grande  UMETA(DisplayName = "Grande")
	, Pequenya UMETA(DisplayName = "Pequenya")
};
