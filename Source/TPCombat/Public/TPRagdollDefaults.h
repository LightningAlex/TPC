#pragma once
#include "CoreMinimal.h"
#include "TPRagdollDefaults.generated.h"

UENUM(BlueprintType)
enum class ERagdollDirection : uint8
{
	RD_NONE				UMETA(DisplayName = "None"),
	RD_FORWARD			UMETA(DisplayName = "Forward"),
	RD_BACKWARD			UMETA(DisplayName = "Backward"),
	MAX					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EActualForwardVector : uint8
{
	AFV_X			UMETA(DisplayName = "X"),
	AFV_Y			UMETA(DisplayName = "Y"),
	AFV_Z			UMETA(DisplayName = "Z"),
	MAX				UMETA(Hidden)
};