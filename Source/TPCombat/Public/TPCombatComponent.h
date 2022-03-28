// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPCombatValues.h"
#include "TPAttackValues.h"
#include "Animation/AnimMontage.h"
#include "DamagedBone.h"
#include "AbilityCooldown.h"
#include "ChainAbilityData.h"
#include "EquipmentSlot.h"
#include "TPCombatComponent.generated.h"

class UCurveFloat;
class ATPCharacter;
class ATPEquipmentBase;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentOff, FName, UnequippedSocket, UTPCombatComponent*, UnequippedCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentOn, ATPEquipmentBase*, EquippedItem, FName, EquippedSocket, UTPCombatComponent*, EquippedCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCombatHealthChanged, float, NewAmount, float, OldAmount, float, AmountPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCombatStaminaChanged, float, NewAmount, float, OldAmount, float, AmountPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCombatManaChanged, float, NewAmount, float, OldAmount, float, AmountPercent);

UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class TPCOMBAT_API UTPCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Values")
	float BaseValues[ECombatValue::MAX];
	float BonusValues[ECombatValue::MAX];
	UPROPERTY(EditDefaultsOnly, Category = "Values")
	UCurveFloat* LevelExperienceCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float AttackDefenses[EAttackType::MAX];
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float MagicDefenses[EMagicElementType::MAX];
	/*Misleading name, more accurate would be "BoneDefense". This array allows certain bones to have a higher/lower multiplier to the damage, i.e. make the head take two times the damage the torso does.*/
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TArray<FDamagedBone> DamagedBones;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	float KnockbackForceReductionRate;
	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	float KnockbackTreshold;
	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	float MaxKnockbackDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TMap<FName, FEquipmentSlot> CharacterEquipment;

	UPROPERTY(BlueprintReadWrite)
	bool bComboWindowOpen;
	UPROPERTY(BlueprintReadWrite)
	TArray<FChainAbilityData> UsableChainAbilities;

	/*Delegates*/
	UPROPERTY(BlueprintAssignable)
	FOnCombatHealthChanged OnCombatHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnCombatStaminaChanged OnCombatStaminaChanged;
	UPROPERTY(BlueprintAssignable)
	FOnCombatManaChanged OnCombatManaChanged;
	UPROPERTY()
	TArray<FAbilityCooldown> AbilitiesOnCooldown;

private:
	float CurrentValues[ECombatValue::MAX];
	int32 LevelValues[ELevelValue::MAX];

	int32 CurrentExperience;
	float CurrentCombatHealth;
	float CurrentCombatMana;
	float CurrentCombatStamina;
	float NextAbilityStartTime;
	float AccumulatedKnockback;
	bool bComboNext;
	bool bCanUseAbilityInGeneral;
	bool bIsParry;
	bool bIsBreak;

	UPROPERTY(Transient)
	UTPAbility* CurrentlyUsedAbility;
	UPROPERTY(Transient)
	ATPCharacter* CharOwner;
	UPROPERTY(Transient)
	TMap<UAnimMontage*, UTPAbility*> UsedAbilities;

	FOnMontageEnded AbilityMontageEndDel;
	FOnMontageEnded ParryMontageEndDel;
	FOnMontageEnded BreakMontageEndDel;

public:	
	UTPCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	float GetBaseValue(ECombatValue SearchedValue);
	UFUNCTION(BlueprintPure)
	float GetCurrentValue(ECombatValue SearchedValue);
	UFUNCTION(BlueprintPure)
	float GetBonusValue(ECombatValue SearchedValue);
	/*Returns current + bonus value*/
	UFUNCTION(BlueprintPure)
	float GetFullValue(ECombatValue SearchedValue);
	UFUNCTION(BlueprintPure)
	int32 GetLevelValue(ELevelValue SearchedValue);
	UFUNCTION(BlueprintCallable)
	void IncreaseLevel(ELevelValue IncreasedValue);
	UFUNCTION(BlueprintCallable)
	void DecreaseLevel(ELevelValue DecreasedValue);
	UFUNCTION(BlueprintPure)
	int32 GetCurrentCombatLevel();
	UFUNCTION(BlueprintPure)
	bool CanLevelUp();

	UFUNCTION(BlueprintCallable)
	void FullHeal();
	UFUNCTION(BlueprintCallable)
	void TakeHit(float InDamage, float InForce, FVector InForceDirection, EAttackType InAttackType, EMagicElementType InElementType, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	void ChangeHealth(float InChange, bool bReplace = false, bool bBroadcast = true);
	UFUNCTION(BlueprintCallable)
	void ChangeMana(float InChange, bool bReplace = false, bool bBroadcast = true);
	UFUNCTION(BlueprintCallable)
	void ChangeStamina(float InChange, bool bReplace = false, bool bBroadcast = true);

	UFUNCTION(BlueprintCallable)
	bool TryUseAbility(TSubclassOf<UTPAbility> UsedAbilityClass);

	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<ATPEquipmentBase> EquipClass, const FName& EquipSlotName);
	UFUNCTION(BlueprintCallable)
	void UnequipItem(const FName& UnequipSlotName, bool bDestroy = true);
	UFUNCTION(BlueprintPure)
	ATPEquipmentBase* GetEquippedItemBySlot(const FName& EquipSlotName);
	UFUNCTION(BlueprintPure)
	ATPEquipmentBase* GetEquippedItemBySocket(const FName& EquipSocket);

	UFUNCTION(BlueprintCallable)
	void SetCanUseAbilityInGeneral(bool bNewCanUse);

	UFUNCTION(BlueprintCallable)
	void ParryAttack(const FName& ParriedSlot, EWeaponType InWT);
	UFUNCTION(BlueprintCallable)
	void BreakBlock(const FName& BrokenSlot, EWeaponType InWT);

	/*Inlines*/
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentExperience() const { return CurrentExperience; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth() const { return CurrentCombatHealth; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentMana() const { return CurrentCombatMana; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentStamina() const { return CurrentCombatStamina; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetHealthNormal() { return (GetCurrentHealth() / GetFullValue(ECombatValue::ECV_HEALTH)); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetManaNormal() { return (GetCurrentMana() / GetFullValue(ECombatValue::ECV_MANA)); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetStaminaNormal() { return (GetCurrentStamina() / GetFullValue(ECombatValue::ECV_STAMINA)); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetCanUseAbilityInGeneral() const { return bCanUseAbilityInGeneral; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE ATPCharacter* GetOwnerAsTP() const { return CharOwner; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsBeingParried() const { return bIsParry; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsBroken() const { return bIsBreak; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UTPAbility* GetCurrentAbility() { return CurrentlyUsedAbility; }

protected:
	virtual void BeginPlay() override;

private:
	void SetCurrentValues();
	bool CanUseAbility(TSubclassOf<UTPAbility> TriedAbilityClass);
	void OnAbilityMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	void OnParryMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	void OnBreakMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	void EndAbilityState(UTPAbility* InAbility, bool bInterrupted);
	void UnequipItem(ATPEquipmentBase* ItemToUnequip, bool bDestroy = true);
	void UnequipItem(FEquipmentSlot& SlotToUnequip, bool bDestroy = true);
};
