// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TPAttackValues.h"
#include "ActiveWeaponState.generated.h"

/**
 * 
 */
UCLASS()
class TPCOMBAT_API UActiveWeaponState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName SocketToActivate;
	UPROPERTY(EditAnywhere)
	TArray<FName> CollidersToActivate;
	UPROPERTY(EditAnywhere)
	EAttackType WeaponAttackType;
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
