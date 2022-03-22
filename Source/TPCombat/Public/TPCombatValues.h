#pragma once
#include "CoreMinimal.h"
#include "TPCombatValues.generated.h"

UENUM(BlueprintType)
enum class ECombatValue : uint8
{
	ECV_HEALTH			UMETA(DisplayName = "Health"),
	ECV_MANA			UMETA(DisplayName = "Mana"),
	ECV_STAMINA			UMETA(DisplayName = "Stamina"),
	ECV_ATTACK			UMETA(DisplayName = "Attack"),
	ECV_M_ATTACK		UMETA(DisplayName = "MagicAttack"),
	ECV_DEFENSE			UMETA(DisplayName = "Defense"),
	ECV_M_DEFENSE		UMETA(DisplayName = "MagicDefense"),
	ECV_POISE			UMETA(DisplayName = "Poise"),
	MAX					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ELevelValue : uint8
{
	ELV_VITALITY			UMETA(DisplayName = "Vitality"),
	ELV_ENDURANCE			UMETA(DisplayName = "Endurance"),
	ELV_STRENGTH			UMETA(DisplayName = "Strength"),
	ELV_DEXTERITY			UMETA(DisplayName = "Dexterity"),
	ELV_INTELLIGENCE		UMETA(DisplayName = "Intelligence"),
	ELV_RESISTANCE			UMETA(DisplayName = "Resistance"),
	MAX						UMETA(Hidden)
};