// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifyQueue.h"
#include "AbilityNotify.generated.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;

UCLASS()
class TPCOMBAT_API UAbilityNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 NotifyIndex;
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
