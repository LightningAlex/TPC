#pragma once
#include "CoreMinimal.h"
#include "TPAbilityDefaults.generated.h"

UENUM(BlueprintType)
enum class EAbilityStaminaBehavior : uint8
{
	ASB_REQUIREDAMOUNT			UMETA(DisplayName = "RequiredAmount"),
	ASB_DRAINTOZERO				UMETA(DisplayName = "DrainToZero"),
	ASB_DRAINBELOWZERO			UMETA(DisplayName = "DrainBelowZero"),
	MAX							UMETA(Hidden)
};