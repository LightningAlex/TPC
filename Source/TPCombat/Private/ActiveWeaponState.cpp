// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveWeaponState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "TPCombatComponent.h"
#include "TPWeaponBase.h"

void UActiveWeaponState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp->GetOwner() != nullptr)
	{
		UTPCombatComponent* OwnerCombat = Cast<UTPCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UTPCombatComponent::StaticClass()));
		if (OwnerCombat)
		{
			ATPWeaponBase* PotentialWeapon = Cast<ATPWeaponBase>(OwnerCombat->GetEquippedItemBySlot(SlotToActivate));
			if (PotentialWeapon)
			{
				PotentialWeapon->ActivateWeapon(CollidersToActivate, WeaponAttackType);
			}
		}
	}
}

void UActiveWeaponState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp->GetOwner() != nullptr)
	{
		UTPCombatComponent* OwnerCombat = Cast<UTPCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UTPCombatComponent::StaticClass()));
		if (OwnerCombat)
		{
			ATPWeaponBase* PotentialWeapon = Cast<ATPWeaponBase>(OwnerCombat->GetEquippedItemBySlot(SlotToActivate));
			if (PotentialWeapon)
			{
				PotentialWeapon->DeactivateWeapon();
			}
		}
	}
}
