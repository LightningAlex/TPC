// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPEquipmentBase.h"
#include "TPArmorBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TPCOMBAT_API ATPArmorBase : public ATPEquipmentBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ArmorMesh;

public:
	ATPArmorBase();

};
