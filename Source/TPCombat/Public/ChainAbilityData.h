#pragma once

#include "CoreMinimal.h"
#include "ChainAbilityData.generated.h"

class UTPAbility;

USTRUCT(BlueprintType)
struct TPCOMBAT_API FChainAbilityData
{
	GENERATED_BODY()

public:
	FChainAbilityData()
	{
	}

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTPAbility> ChainAbilityClass;
	UPROPERTY(EditDefaultsOnly)
	float ChainAbilityStartTime;

	bool operator==(const FChainAbilityData& rhs) const;
};
