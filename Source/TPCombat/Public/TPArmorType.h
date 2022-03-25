#pragma once
#include "CoreMinimal.h"
#include "TPArmorType.generated.h"

/*Deprecated, can be deleted*/
UENUM(BlueprintType)
enum class EArmorType : uint8
{
	AT_HELMET		UMETA(DisplayName = "Helmet"),
	AT_CHEST		UMETA(DisplayName = "Chest"),
	AT_LEGGINGS		UMETA(DisplayName = "Leggings"),
	AT_FOOTWEAR		UMETA(DisplayName = "Footwear"),
	AT_GAUNTLETS	UMETA(DisplayName = "Gauntlet"),
	AT_NECKLACE		UMETA(DisplayName = "Necklace"),
	AT_RING			UMETA(DisplayName = "Ring"),
	MAX
};