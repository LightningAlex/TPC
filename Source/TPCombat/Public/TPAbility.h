// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TPAttackValues.h"
#include "GameplayTagContainer.h"
#include "TPAbilityDefaults.h"
#include "TPAbility.generated.h"

class UAnimMontage;
class UTPCombatComponent;

UCLASS(BlueprintType, Blueprintable)
class TPCOMBAT_API UTPAbility : public UObject
{
	GENERATED_BODY()
	
public:
	/*Used for comparisons*/
	UPROPERTY(EditDefaultsOnly, Category = "Ability Data")
	FName AbilityName;
	/*Used for UI*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Data")
	FText AbilityDisplayName;
	/*Used for UI*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Data")
	FText AbilityDisplayDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	float AbilityHealthCost;
	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	float AbilityManaCost;
	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	float AbilityStaminaCost;
	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	EAbilityStaminaBehavior AbilityStaminaBehavior;

	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	float MovementSpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	bool bCanBeUsedInAir;
	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	bool bCanAbilityTick;
	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	float AbilityCooldownDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	FGameplayTag AbilityComboTag;
	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TSubclassOf<UTPAbility> ComboAbilityClass;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AbilityStart();

	UFUNCTION(BlueprintImplementableEvent)
	void AbilityTick(float XDeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void AbilityEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void AbilityNotify(int32 NotifyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void AbilityInterrupted();

	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* GetAbilityMontage(UTPCombatComponent* UsedCC);
};
