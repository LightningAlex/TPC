#pragma once
#include "CoreMinimal.h"
#include "TPAttackValues.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	AT_SLASH			UMETA(DisplayName = "Slash"),
	AT_STAB				UMETA(DisplayName = "Stab"),
	AT_BLUNT			UMETA(DisplayName = "Blunt"),
	AT_SPECIAL			UMETA(DisplayName = "Special"),
	MAX					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMagicElementType : uint8
{
	MET_NONE			UMETA(DisplayName = "None"),
	MET_FIRE			UMETA(DisplayName = "Fire"),
	MET_LIGHTNING		UMETA(DisplayName = "Lightning"),
	MET_LIFE			UMETA(DisplayName = "Life"),
	MET_WIND			UMETA(DisplayName = "Wind"),
	MET_WATER			UMETA(DisplayName = "Water"),
	MET_ICE				UMETA(DisplayName = "Ice"),
	MET_EARTH			UMETA(DisplayName = "Earth"),
	MAX					UMETA(Hidden)
};