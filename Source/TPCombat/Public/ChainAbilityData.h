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

	FChainAbilityData(TSubclassOf<UTPAbility>& InChainAbilityClass)
		:ChainAbilityClass(InChainAbilityClass)
	{
	}

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTPAbility> ChainAbilityClass;
	UPROPERTY(EditDefaultsOnly)
	float ChainAbilityStartTime;

	bool operator==(const FChainAbilityData& rhs) const
	{
		return ChainAbilityClass == rhs.ChainAbilityClass;
	}
};
