#pragma once
#include "CoreMinimal.h"
#include "AbilityContainer.generated.h"

class UTPAbility;

/*NOTE: THIS STRUCT ISN'T USED ANYMORE AND CAN BE SAFELY DELETED!*/

USTRUCT(BlueprintType)
struct TPCOMBAT_API FAbilityContainer
{
	GENERATED_BODY()

public:
	FAbilityContainer()
	{
	}

	FAbilityContainer(const FName& InName)
		:ContainerName(InName)
	{
	}

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName ContainerName;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TSubclassOf<UTPAbility>> ContainedAbilities;

public:
	TSubclassOf<UTPAbility> GetContainedAbility(const FName& InAbilityName);
	TSubclassOf<UTPAbility>& GetContainedAbility(int32 InAbilityIndex);
};