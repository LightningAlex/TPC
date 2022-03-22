// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPEquipmentBase.h"
#include "TPArmorType.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorType ArmorType;

public:
	ATPArmorBase();
	UFUNCTION(BlueprintPure)
	bool ShouldSetMasterComponent() const;
};
