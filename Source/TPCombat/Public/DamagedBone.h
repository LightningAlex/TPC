#pragma once
#include "CoreMinimal.h"
#include "DamagedBone.generated.h"

USTRUCT(BlueprintType)
struct FDamagedBone
{
	GENERATED_BODY()

public:
	FDamagedBone()
		:DamageMultiplier(1.f)
	{
	}

public:
	UPROPERTY(EditDefaultsOnly)
	FName BoneName;
	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier;

	bool operator==(const FDamagedBone& rhs) const
	{
		return BoneName == rhs.BoneName;
	}
};