// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "TPCharacter.h"
#include "TPCombatComponent.h"

void UAbilityNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ATPCharacter* TPChar = Cast<ATPCharacter>(MeshComp->GetOwner());
	if (TPChar && TPChar->TPCombatComponent && TPChar->TPCombatComponent->GetCurrentAbility())
	{
		TPChar->TPCombatComponent->GetCurrentAbility()->AbilityNotify(NotifyIndex);
	}
}
