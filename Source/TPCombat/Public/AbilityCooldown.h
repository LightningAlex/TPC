#pragma once
#include "CoreMinimal.h"
#include "AbilityCooldown.generated.h"

USTRUCT(BlueprintType)
struct FAbilityCooldown
{
	GENERATED_BODY()

public:
	FAbilityCooldown()
	{
	}

	FAbilityCooldown(const FName& InName)
		:CooldownAbilityName(InName)
	{
	}

	FAbilityCooldown(const FName& InName, float InDuration)
		:CooldownAbilityName(InName),
		RemainingCooldown(InDuration)
	{
	}

private:
	UPROPERTY()
	FName CooldownAbilityName;
	UPROPERTY()
	float RemainingCooldown;

public:
	bool operator==(const FAbilityCooldown& rhs) const
	{
		return CooldownAbilityName == rhs.CooldownAbilityName;
	}

	bool TickCooldown(float XDeltaTime)
	{
		RemainingCooldown -= XDeltaTime;
		return RemainingCooldown < 0.f;
	}
};