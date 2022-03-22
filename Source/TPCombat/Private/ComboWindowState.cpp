// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboWindowState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "TPCombatComponent.h"

void UComboWindowState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp->GetOwner() != nullptr)
	{
		UTPCombatComponent* OwnerCombat = Cast<UTPCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UTPCombatComponent::StaticClass()));
		if (OwnerCombat)
		{
			OwnerCombat->bComboWindowOpen = true;
		}
	}
}

void UComboWindowState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp->GetOwner() != nullptr)
	{
		UTPCombatComponent* OwnerCombat = Cast<UTPCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UTPCombatComponent::StaticClass()));
		if (OwnerCombat)
		{
			OwnerCombat->bComboWindowOpen = false;
		}
	}
}
