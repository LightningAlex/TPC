#pragma once
#include "CoreMinimal.h"
#include "TPWeaponDefaults.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_BARE			UMETA(DisplayName = "Bare"),
	WT_SHORTSWORD	UMETA(DisplayName = "ShortSword"),
	WT_LONGSWORD	UMETA(DisplayName = "LongSword"),
	WT_GREATSWORD	UMETA(DisplayName = "Greatsword"),
	WT_KATANA		UMETA(DisplayName = "Katana"),
	WT_SPEAR		UMETA(DisplayName = "Spear"),
	WT_SHORTAXE		UMETA(DisplayName = "ShortAxe"),
	WT_GREATAXE		UMETA(DisplayName = "GreatAxe"),
	WT_SCYTHE		UMETA(DisplayName = "Scythe"),
	WT_GUANDAO		UMETA(DisplayName = "Guandao"),
	MAX				UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponScalabilityFactor : uint8
{
	WSF_A		UMETA(DisplayName = "A"),
	WSF_B		UMETA(DisplayName = "B"),
	WSF_C		UMETA(DisplayName = "C"),
	WSF_D		UMETA(DisplayName = "D"),
	MAX			UMETA(Hidden)
};