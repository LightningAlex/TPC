// Fill out your copyright notice in the Description page of Project Settings.


#include "TPArmorBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "TPCombatComponent.h"
#include "GameFramework/Character.h"

ATPArmorBase::ATPArmorBase()
	:Super()
{
	ArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmorMesh"));
	ArmorMesh->SetupAttachment(GetRootComponent());
}

