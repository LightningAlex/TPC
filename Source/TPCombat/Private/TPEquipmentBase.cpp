// Fill out your copyright notice in the Description page of Project Settings.


#include "TPEquipmentBase.h"
#include "TPCombatComponent.h"
#include "TPCharacter.h"
#include "Components/SceneComponent.h"

// Sets default values
ATPEquipmentBase::ATPEquipmentBase()
{
	PrimaryActorTick.bCanEverTick = false;

	EquipmentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("EquipmentRoot"));
	SetRootComponent(EquipmentRoot);
}

void ATPEquipmentBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPEquipmentBase::BeEquipped(UTPCombatComponent* InCombatComponent, FName InSlotName /*= NAME_None*/)
{
	for (int32 i = 0; i < static_cast<int32>(ECombatValue::MAX); i++)
	{
		InCombatComponent->BonusValues[i] += AddedValues[i];
	}
	AttachedToSlot = InSlotName;
	EquipmentHolder = Cast<ATPCharacter>(InCombatComponent->GetOwner());
	Event_Equipped(InCombatComponent, InSlotName);
}

void ATPEquipmentBase::BeUnequipped(UTPCombatComponent* InCombatComponent)
{
	AttachedToSlot = NAME_None;
	for (int32 i = 0; i < static_cast<int32>(ECombatValue::MAX); i++)
	{
		InCombatComponent->BonusValues[i] -= AddedValues[i];
	}
	Event_Unequipped(InCombatComponent);
}

float ATPEquipmentBase::GetAddedValue(ECombatValue SearchedValue)
{
	return AddedValues[static_cast<int32>(SearchedValue)];
}

// Called when the game starts or when spawned
void ATPEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
	
}


