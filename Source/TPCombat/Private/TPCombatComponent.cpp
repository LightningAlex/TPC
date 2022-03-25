// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCombatComponent.h"
#include "Curves/CurveFloat.h"
#include "TPAbility.h"
#include "TPCharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "TPCharacter.h"
#include "TPArmorBase.h"
#include "TPWeaponBase.h"

bool FChainAbilityData::operator==(const FChainAbilityData& rhs) const
{
	return ChainAbilityClass == rhs.ChainAbilityClass;
}

UTPCombatComponent::UTPCombatComponent()
	:BaseValues{10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f},
	AttackDefenses{1.f, 1.f, 1.f, 1.f},
	MagicDefenses{1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
	KnockbackForceReductionRate(15.f),
	KnockbackTreshold(0.5f),
	MaxKnockbackDuration(3.f),
	LevelValues{1, 1, 1, 1, 1, 1},
	bCanUseAbilityInGeneral(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}

void UTPCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CharOwner = Cast<ATPCharacter>(GetOwner());
	SetCurrentValues();
}

// Called every frame
void UTPCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AbilitiesOnCooldown.Num() > 0)
	{
		for (int32 i = AbilitiesOnCooldown.Num() - 1; i >= 0; i--)
		{
			if (AbilitiesOnCooldown[i].TickCooldown(DeltaTime))
			{
				AbilitiesOnCooldown.RemoveAt(i);
			}
		}
	}

	if (CurrentlyUsedAbility != nullptr && CurrentlyUsedAbility->bCanAbilityTick)
	{
		CurrentlyUsedAbility->AbilityTick(DeltaTime);
	}

	if (AccumulatedKnockback > 0.f)
	{
		AccumulatedKnockback = FMath::Max(AccumulatedKnockback - DeltaTime * KnockbackForceReductionRate, 0.f);
	}
}

float UTPCombatComponent::GetBaseValue(ECombatValue SearchedValue)
{
	return BaseValues[static_cast<int32>(SearchedValue)];
}

float UTPCombatComponent::GetCurrentValue(ECombatValue SearchedValue)
{
	return CurrentValues[static_cast<int32>(SearchedValue)];
}

float UTPCombatComponent::GetBonusValue(ECombatValue SearchedValue)
{
	return BonusValues[static_cast<int32>(SearchedValue)];
}

float UTPCombatComponent::GetFullValue(ECombatValue SearchedValue)
{
	return GetCurrentValue(SearchedValue) + GetBonusValue(SearchedValue);
}

int32 UTPCombatComponent::GetLevelValue(ELevelValue SearchedValue)
{
	return LevelValues[static_cast<int32>(SearchedValue)];
}

void UTPCombatComponent::IncreaseLevel(ELevelValue IncreasedValue)
{
	LevelValues[static_cast<int32>(IncreasedValue)]++;
	CurrentExperience -= static_cast<int32>(LevelExperienceCurve->GetFloatValue(static_cast<float>(GetCurrentCombatLevel())));
	SetCurrentValues();
}

void UTPCombatComponent::DecreaseLevel(ELevelValue DecreasedValue)
{
	LevelValues[static_cast<int32>(DecreasedValue)]--;
	CurrentExperience += static_cast<int32>(LevelExperienceCurve->GetFloatValue(static_cast<float>(GetCurrentCombatLevel())));
	SetCurrentValues();
}

int32 UTPCombatComponent::GetCurrentCombatLevel()
{
	int32 RetVal = 0;
	for (int32 i = 0; i < static_cast<int32>(ELevelValue::MAX); i++)
	{
		RetVal += GetLevelValue(static_cast<ELevelValue>(i));
	}
	return RetVal;
}

bool UTPCombatComponent::CanLevelUp()
{
	return LevelExperienceCurve->GetFloatValue(static_cast<float>(GetCurrentCombatLevel())) > static_cast<float>(CurrentExperience);
}

void UTPCombatComponent::FullHeal()
{
	ChangeHealth(BIG_NUMBER, true);
	ChangeStamina(BIG_NUMBER, true);
	ChangeMana(BIG_NUMBER, true);
}

void UTPCombatComponent::TakeHit(float InDamage, float InForce, FVector InForceDirection, EAttackType InAttackType, EMagicElementType InElementType, const FHitResult& Hit)
{
	if (InDamage > 0.f)
	{
		/*CALCULATE EXACT DAMAGE*/
		/*Check weaknesses, resistances and immunities*/
		const float AttackTypeDefense = AttackDefenses[static_cast<int32>(InAttackType)];
		const float MagicTypeDefense = MagicDefenses[static_cast<int32>(InElementType)];
		
		if (AttackTypeDefense == 0.f || MagicTypeDefense == 0.f) { return; }

		InDamage *= AttackTypeDefense;
		InDamage *= MagicTypeDefense;

		for (int32 i = 0; i < DamagedBones.Num(); i++)
		{
			if (DamagedBones[i].BoneName == Hit.BoneName)
			{
				InDamage *= DamagedBones[i].DamageMultiplier;
				break;
			}
		}

		/*Factor in defense and magical defense*/
		if (InAttackType == EAttackType::AT_SPECIAL)
		{
			float PercentReduction = FMath::Sqrt(GetFullValue(ECombatValue::ECV_M_DEFENSE));
			if (PercentReduction > 100.f)
			{
				PercentReduction = 0.8f;
			}
			else
			{
				while (PercentReduction > 1.f)
				{
					PercentReduction *= 0.1f;
				}
			}
			InDamage -= PercentReduction * InDamage;
		}
		else
		{
			float PercentReduction = FMath::Sqrt(GetFullValue(ECombatValue::ECV_DEFENSE));
			if (PercentReduction > 100.f)
			{
				PercentReduction = 0.8f;
			}
			else
			{
				while (PercentReduction > 1.f)
				{
					PercentReduction *= 0.1f;
				}
			}
			InDamage -= PercentReduction * InDamage;
		}
		InDamage += InDamage * FMath::RandRange(-0.05f, 0.05f);
		ChangeHealth(FMath::Max(0.f, InDamage * (-1.f)));
	}
	if (InForce > 0.f)
	{
		AccumulatedKnockback += InForce;
		if (AccumulatedKnockback > GetFullValue(ECombatValue::ECV_POISE) * KnockbackTreshold && CharOwner != nullptr)
		{
			if (CurrentlyUsedAbility != nullptr)
			{
				UAnimMontage* AbilityMontage = CurrentlyUsedAbility->GetAbilityMontage(this);
				check(AbilityMontage);
				if (CharOwner->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AbilityMontage))
				{
					CharOwner->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, AbilityMontage);
				}
				else
				{
					EndAbilityState(CurrentlyUsedAbility, true);
				}
			}
			CharOwner->StartRagdoll(FMath::Min(MaxKnockbackDuration, AccumulatedKnockback / GetFullValue(ECombatValue::ECV_POISE)), InForceDirection * AccumulatedKnockback, Hit.BoneName);
			AccumulatedKnockback = 0.f;
		}
	}
}

bool UTPCombatComponent::TryUseAbility(TSubclassOf<UTPAbility> UsedAbilityClass)
{
	if (CanUseAbility(UsedAbilityClass))
	{
		CurrentlyUsedAbility = NewObject<UTPAbility>(this, bComboNext ? CurrentlyUsedAbility->ComboAbilityClass : UsedAbilityClass, NAME_None);
		bComboNext = false;
		UAnimMontage* UsedAbilityMontage = CurrentlyUsedAbility->GetAbilityMontage(this);
		check(UsedAbilityMontage);

		AbilityMontageEndDel.BindUObject(this, &UTPCombatComponent::OnAbilityMontageEnd);
		CharOwner->GetMesh()->GetAnimInstance()->Montage_Play(UsedAbilityMontage, 1.f, EMontagePlayReturnType::MontageLength, NextAbilityStartTime);
		NextAbilityStartTime = 0.f;
		CharOwner->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(AbilityMontageEndDel, UsedAbilityMontage);
		CurrentlyUsedAbility->AbilityStart();
		CharOwner->GetTPCharacterMovement()->SetMaxMovementMultiplier(CurrentlyUsedAbility->MovementSpeedMultiplier);
		if (CurrentlyUsedAbility->AbilityHealthCost > 0.f)
		{
			ChangeHealth(CurrentlyUsedAbility->AbilityHealthCost * (-1.f));
		}
		if (CurrentlyUsedAbility->AbilityManaCost > 0.f)
		{
			ChangeMana(CurrentlyUsedAbility->AbilityManaCost);
		}
		if (CurrentlyUsedAbility->AbilityStaminaCost > 0.f)
		{
			ChangeStamina(CurrentlyUsedAbility->AbilityStaminaCost);
		}
		UsedAbilities.Add(UsedAbilityMontage, CurrentlyUsedAbility);

		if (CurrentlyUsedAbility->AbilityCooldownDuration > 0.f) { AbilitiesOnCooldown.Add(FAbilityCooldown(CurrentlyUsedAbility->AbilityName, CurrentlyUsedAbility->AbilityCooldownDuration)); }

		return true;
	}
	return false;
}

void UTPCombatComponent::EquipItem(TSubclassOf<ATPEquipmentBase> EquipClass, const FName& EquipSlot)
{
	ATPEquipmentBase** AlreadyEqupped = CharacterEquipment.Find(EquipSlot);
	if (AlreadyEqupped)
	{
		if (*AlreadyEqupped)
		{
			UnequipItem(*AlreadyEqupped);
		}

		FActorSpawnParameters EquipSpawnParams;
		EquipSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATPCharacter* TPOwner = GetOwnerAsTP();

		ATPEquipmentBase* NewEquipped = GetWorld()->SpawnActor<ATPEquipmentBase>(EquipClass,
			TPOwner->GetMesh()->GetSocketTransform(EquipSlot,
				ERelativeTransformSpace::RTS_World), EquipSpawnParams);

		ATPArmorBase* EquipAsArmor = Cast<ATPArmorBase>(NewEquipped);
		if (EquipAsArmor)
		{
			if (EquipAsArmor->ShouldSetMasterComponent())
			{
				EquipAsArmor->ArmorMesh->SetMasterPoseComponent(TPOwner->GetMesh());
			}
		}

		NewEquipped->AttachToComponent(TPOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false), EquipSlot);
		NewEquipped->BeEquipped(this, EquipSlot);
		*AlreadyEqupped = NewEquipped;
	}
}

void UTPCombatComponent::UnequipItem(const FName& UnequipSlot)
{
	ATPEquipmentBase** CurrentEquipped = CharacterEquipment.Find(UnequipSlot);
	if (CurrentEquipped)
	{
		if (*CurrentEquipped)
		{
			UnequipItem(*CurrentEquipped);
		}
	}
}

void UTPCombatComponent::UnequipItem(ATPEquipmentBase*& ItemToUnequip)
{
	ItemToUnequip->BeUnequipped(this);
	ItemToUnequip->Destroy();
	ItemToUnequip = nullptr;
}

ATPEquipmentBase* UTPCombatComponent::GetEquippedItem(const FName& EquipSlot)
{
	ATPEquipmentBase** CurrentEquipped = CharacterEquipment.Find(EquipSlot);
	if (CurrentEquipped)
	{
		return *CurrentEquipped;
	}
	return nullptr;
}

bool UTPCombatComponent::CanUseAbility(TSubclassOf<UTPAbility> TriedAbilityClass)
{
	/*If bComboWindowOpen is true, CurrentlyUsedAbility MUST NOT be a nullptr. If it is, it's a bug and something needs to be fixed.*/
	bool ComboPassesCheck = false;
	if (bComboWindowOpen &&
		CurrentlyUsedAbility->ComboAbilityClass.GetDefaultObject()->AbilityComboTag.MatchesTagExact(TriedAbilityClass.GetDefaultObject()->AbilityComboTag) &&
		CurrentlyUsedAbility->ComboAbilityClass->IsValidLowLevel())
	{
		TriedAbilityClass = CurrentlyUsedAbility->ComboAbilityClass;
		ComboPassesCheck = true;
	}

	if (bCanUseAbilityInGeneral && !bIsParry && !bIsBreak && TriedAbilityClass.GetDefaultObject()->AbilityHealthCost < GetCurrentHealth() &&
		TriedAbilityClass.GetDefaultObject()->AbilityManaCost < GetCurrentMana() && !CharOwner->IsRagdoll() &&
		TriedAbilityClass.GetDefaultObject()->AbilityStaminaCost < GetCurrentStamina() && !CharOwner->IsRecoveringFromRagdoll() &&
		!AbilitiesOnCooldown.Contains(FAbilityCooldown(TriedAbilityClass.GetDefaultObject()->AbilityName)))
	{
		if (!CharOwner->GetTPCharacterMovement()->IsFalling() || TriedAbilityClass.GetDefaultObject()->bCanBeUsedInAir)
		{
			if (CurrentlyUsedAbility)
			{
				/*Check for chain/combo*/
				if (ComboPassesCheck)
				{
					/*Other checks, such as the equality of the tag and the existence of the combo ability are done above*/
					bComboNext = true;
					return true;
				}
				if (UsableChainAbilities.Num() > 0)
				{
					int32 FoundChainAbilityIndex = -1;
					for (int32 i = 0; i < UsableChainAbilities.Num(); i++)
					{
						if (UsableChainAbilities[i].ChainAbilityClass == TriedAbilityClass)
						{
							FoundChainAbilityIndex = i;
							break;
						}
					}
					if (FoundChainAbilityIndex > -1)
					{
						NextAbilityStartTime = UsableChainAbilities[FoundChainAbilityIndex].ChainAbilityStartTime;
						return true;
					}
				}
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

void UTPCombatComponent::OnAbilityMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (bComboWindowOpen) { bComboWindowOpen = false; }
	if (UsableChainAbilities.Num() > 0) { UsableChainAbilities.Reset(); }

	/*This function is structured like this because `CurrentlyUsedAbility` cannot be used to call events on it. Animations can be interrupted*/
	/*when other abilities are used, and when that happens, CurrentlyUsedAbility (which is set as soon as the new one starts)*/
	/*isn't necessarily the same ability whose animation ended. */
	UTPAbility** EndingAbility = UsedAbilities.Find(Montage);
	if (EndingAbility)
	{
		EndAbilityState(*EndingAbility, bInterrupted);
	}
}

void UTPCombatComponent::OnParryMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsParry = false;
}

void UTPCombatComponent::OnBreakMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsBreak = false;
}

void UTPCombatComponent::EndAbilityState(UTPAbility* InAbility, bool bInterrupted)
{
	if (InAbility)
	{
		if (bInterrupted)
		{
			InAbility->AbilityInterrupted();
		}
		else
		{
			InAbility->AbilityEnd();
		}
		if (InAbility == CurrentlyUsedAbility)
		{
			CharOwner->GetTPCharacterMovement()->RestoreOriginalWalkingSpeed();
			CurrentlyUsedAbility = nullptr;
		}
		UsedAbilities.Remove(InAbility->GetAbilityMontage(this));
	}
}

void UTPCombatComponent::SetCanUseAbilityInGeneral(bool bNewCanUse)
{
	bCanUseAbilityInGeneral = bNewCanUse;
}

void UTPCombatComponent::ParryAttack(const FName& ParriedSocket, EWeaponType InWT)
{
	ATPCharacter* TPCOwner = GetOwnerAsTP();
	check (TPCOwner);

	UAnimMontage* ParryMontage = TPCOwner->GetParryMontage(ParriedSocket, InWT);

	bIsParry = true;
	ParryMontageEndDel.BindUObject(this, &UTPCombatComponent::OnParryMontageEnd);
	TPCOwner->GetMesh()->GetAnimInstance()->Montage_Play(ParryMontage);
	TPCOwner->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(ParryMontageEndDel, ParryMontage);
}

void UTPCombatComponent::BreakBlock(const FName& BrokenSocket, EWeaponType InWT)
{
	ATPCharacter* TPCOwner = GetOwnerAsTP();
	check(TPCOwner);

	UAnimMontage* BreakMontage = TPCOwner->GetBlockBreakMontage(BrokenSocket, InWT);

	bIsBreak = true;
	BreakMontageEndDel.BindUObject(this, &UTPCombatComponent::OnBreakMontageEnd);
	TPCOwner->GetMesh()->GetAnimInstance()->Montage_Play(BreakMontage);
	TPCOwner->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(BreakMontageEndDel, BreakMontage);

	for (TPair<FName, ATPEquipmentBase*> ObservedEquipment : CharacterEquipment)
	{
		ATPWeaponBase* ObservedWeapon = Cast<ATPWeaponBase>(ObservedEquipment.Value);
		if (ObservedWeapon)
		{
			ObservedWeapon->SetWeaponBlocking(false);
		}
	}
}

void UTPCombatComponent::SetCurrentValues()
{
	for (int32 i = 0; i < static_cast<int32>(ECombatValue::MAX); i++)
	{
		CurrentValues[i] = 0.f;
	}
	for (int32 i = 0; i < static_cast<int32>(ELevelValue::MAX); i++)
	{
		ELevelValue ObservedValue = static_cast<ELevelValue>(i);

		switch (ObservedValue)
		{
		case ELevelValue::ELV_VITALITY:
			CurrentValues[static_cast<int32>(ECombatValue::ECV_HEALTH)] += GetBaseValue(ECombatValue::ECV_HEALTH) * (float)GetLevelValue(ObservedValue) * 2.8f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_STAMINA)] += GetBaseValue(ECombatValue::ECV_STAMINA) * (float)GetLevelValue(ObservedValue) * 1.1f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_MANA)] += GetBaseValue(ECombatValue::ECV_MANA) * (float)GetLevelValue(ObservedValue) * 0.6f;
			break;
		case ELevelValue::ELV_ENDURANCE:
			CurrentValues[static_cast<int32>(ECombatValue::ECV_HEALTH)] += GetBaseValue(ECombatValue::ECV_HEALTH) * (float)GetLevelValue(ObservedValue) * 0.67f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_STAMINA)] += GetBaseValue(ECombatValue::ECV_STAMINA) * (float)GetLevelValue(ObservedValue) * 3.f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_DEFENSE)] += GetBaseValue(ECombatValue::ECV_DEFENSE) * (float)GetLevelValue(ObservedValue) * 0.83f;
			break;
		case ELevelValue::ELV_STRENGTH:
			CurrentValues[static_cast<int32>(ECombatValue::ECV_HEALTH)] += GetBaseValue(ECombatValue::ECV_HEALTH) * (float)GetLevelValue(ObservedValue) * 0.4f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_ATTACK)] += GetBaseValue(ECombatValue::ECV_ATTACK) * (float)GetLevelValue(ObservedValue) * 2.4f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_DEFENSE)] += GetBaseValue(ECombatValue::ECV_DEFENSE) * (float)GetLevelValue(ObservedValue) * 0.8f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_POISE)] += GetBaseValue(ECombatValue::ECV_POISE) * (float)GetLevelValue(ObservedValue) * 0.9f;
			break;
		case ELevelValue::ELV_DEXTERITY:
			CurrentValues[static_cast<int32>(ECombatValue::ECV_HEALTH)] += GetBaseValue(ECombatValue::ECV_HEALTH) * (float)GetLevelValue(ObservedValue) * 0.4f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_ATTACK)] += GetBaseValue(ECombatValue::ECV_ATTACK) * (float)GetLevelValue(ObservedValue) * 2.1f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_M_ATTACK)] += GetBaseValue(ECombatValue::ECV_M_ATTACK) * (float)GetLevelValue(ObservedValue) * 1.25f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_M_DEFENSE)] += GetBaseValue(ECombatValue::ECV_M_DEFENSE) * (float)GetLevelValue(ObservedValue) * 0.75f;
			break;
		case ELevelValue::ELV_INTELLIGENCE:
			CurrentValues[static_cast<int32>(ECombatValue::ECV_HEALTH)] += GetBaseValue(ECombatValue::ECV_HEALTH) * (float)GetLevelValue(ObservedValue) * 0.2f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_MANA)] += GetBaseValue(ECombatValue::ECV_MANA) * (float)GetLevelValue(ObservedValue) * 1.2f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_M_ATTACK)] += GetBaseValue(ECombatValue::ECV_M_ATTACK) * (float)GetLevelValue(ObservedValue) * 2.5f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_M_DEFENSE)] += GetBaseValue(ECombatValue::ECV_M_DEFENSE) * (float)GetLevelValue(ObservedValue) * 0.6f;
			break;
		case ELevelValue::ELV_RESISTANCE:
			CurrentValues[static_cast<int32>(ECombatValue::ECV_HEALTH)] += GetBaseValue(ECombatValue::ECV_HEALTH) * (float)GetLevelValue(ObservedValue) * 0.5f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_STAMINA)] += GetBaseValue(ECombatValue::ECV_STAMINA) * (float)GetLevelValue(ObservedValue) * 0.5;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_M_DEFENSE)] += GetBaseValue(ECombatValue::ECV_M_DEFENSE) * (float)GetLevelValue(ObservedValue) * 1.4f;
			CurrentValues[static_cast<int32>(ECombatValue::ECV_POISE)] += GetBaseValue(ECombatValue::ECV_POISE) * (float)GetLevelValue(ObservedValue) * 2.1f;
			break;
		}
	}
}

void UTPCombatComponent::ChangeHealth(float InChange, bool bReplace /*= false*/, bool bBroadcast /*= true*/)
{
	float OldAmount = CurrentCombatHealth;
	CurrentCombatHealth = FMath::Clamp(InChange + (bReplace ? 0.f : CurrentCombatHealth), 0.f, GetFullValue(ECombatValue::ECV_HEALTH));
	if (bBroadcast)
	{
		OnCombatHealthChanged.Broadcast(CurrentCombatHealth, OldAmount, GetHealthNormal());
	}
}

void UTPCombatComponent::ChangeMana(float InChange, bool bReplace /*= false*/, bool bBroadcast /*= true*/)
{
	float OldAmount = CurrentCombatMana;
	CurrentCombatMana = FMath::Clamp(InChange + (bReplace ? 0.f : CurrentCombatMana), 0.f, GetFullValue(ECombatValue::ECV_MANA));
	if (bBroadcast)
	{
		OnCombatManaChanged.Broadcast(CurrentCombatMana, OldAmount, GetManaNormal());
	}
}

void UTPCombatComponent::ChangeStamina(float InChange, bool bReplace /*= false*/, bool bBroadcast /*= true*/)
{
	float OldAmount = CurrentCombatStamina;
	CurrentCombatStamina = FMath::Clamp(InChange + (bReplace ? 0.f : CurrentCombatStamina), 0.f, GetFullValue(ECombatValue::ECV_STAMINA));
	if (bBroadcast)
	{
		OnCombatStaminaChanged.Broadcast(CurrentCombatStamina, OldAmount, GetStaminaNormal());
	}
}
