// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChainAbilityData.h"
#include "TPAttackValues.h"
#include "GameplayTagContainer.h"
#include "TPAbility.generated.h"

class UAnimMontage;
class UTPCombatComponent;

UCLASS(BlueprintType, Blueprintable)
class TPCOMBAT_API UTPAbility : public UObject
{
	GENERATED_BODY()
	
public:
	/*Used for comparisons*/
	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FName AbilityName;
	/*Used for UI*/
	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FText AbilityDisplayName;
	/*Used for UI*/
	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FText AbilityDisplayDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	float AbilityHealthCost;
	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	float AbilityManaCost;
	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	float AbilityStaminaCost;

	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	float MovementSpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	bool bCanBeUsedInAir;
	UPROPERTY(EditDefaultsOnly, Category = "Usage")
	bool bCanAbilityTick;

	UPROPERTY(EditDefaultsOnly, Category = "ComboChain")
	FGameplayTag AbilityComboTag;
	UPROPERTY(EditDefaultsOnly, Category = "ComboChain")
	TSubclassOf<UTPAbility> ComboAbilityClass;
	UPROPERTY(EditDefaultsOnly, Category = "ComboChain")
	TArray<FChainAbilityData> ChainAbilities;

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
