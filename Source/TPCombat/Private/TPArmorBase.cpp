// Fill out your copyright notice in the Description page of Project Settings.


#include "TPArmorBase.h"
#include "Components/SkeletalMeshComponent.h"

ATPArmorBase::ATPArmorBase()
	:Super()
{
	ArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmorMesh"));
	ArmorMesh->SetupAttachment(GetRootComponent());
}

bool ATPArmorBase::ShouldSetMasterComponent() const
{
	return (ArmorType != EArmorType::AT_NECKLACE && ArmorType != EArmorType::AT_RING);
}
