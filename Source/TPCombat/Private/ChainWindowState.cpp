// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainWindowState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "TPCombatComponent.h"

void UChainWindowState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp->GetOwner() != nullptr)
	{
		UTPCombatComponent* OwnerCombat = Cast<UTPCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UTPCombatComponent::StaticClass()));
		if (OwnerCombat)
		{
			OwnerCombat->UsableChainAbilities.Append(UsableChains);
			OwnerCombat->DebugPrintUsableChains();
		}
	}
}

void UChainWindowState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp->GetOwner() != nullptr)
	{
		UTPCombatComponent* OwnerCombat = Cast<UTPCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UTPCombatComponent::StaticClass()));
		if (OwnerCombat)
		{
			for (int32 i = OwnerCombat->UsableChainAbilities.Num() - 1; i >= 0; i--)
			{
				if (UsableChains.Contains(OwnerCombat->UsableChainAbilities[i]))
				{
					OwnerCombat->UsableChainAbilities.RemoveAt(i);
				}
			}
			OwnerCombat->DebugPrintUsableChains();
		}
	}
}
