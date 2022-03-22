#include "AbilityContainer.h"
#include "TPAbility.h"

TSubclassOf<UTPAbility> FAbilityContainer::GetContainedAbility(const FName& InAbilityName)
{
	for (TSubclassOf<UTPAbility>& ObservedAbilityClass : ContainedAbilities)
	{
		if (ObservedAbilityClass.GetDefaultObject()->AbilityName == InAbilityName)
		{
			return ObservedAbilityClass;
		}
	}
	return nullptr;
}

TSubclassOf<UTPAbility>& FAbilityContainer::GetContainedAbility(int32 InAbilityIndex)
{
	return ContainedAbilities[InAbilityIndex];
}
