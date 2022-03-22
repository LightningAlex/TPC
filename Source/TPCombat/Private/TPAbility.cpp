// Fill out your copyright notice in the Description page of Project Settings.

#include "TPAbility.h"
#include "TPCombatComponent.h"

UAnimMontage* UTPAbility::GetAbilityMontage_Implementation(UTPCombatComponent* UsedCC)
{
	if (AbilityAnimations.Num() > 0)
	{
		return AbilityAnimations[0];
	}
	return nullptr;
}
