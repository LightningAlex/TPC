#pragma once
#include "CoreMinimal.h"
#include "EquipmentSlot.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY()

public:
	FEquipmentSlot()
	{
	}

	FEquipmentSlot(const FName& InSlotName, const FName& InSocketName)
	{
		EquipmentSlot = InSlotName;
		EquipmentSocket = InSocketName;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipmentSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipmentSocket;

public:
	bool operator == (const FEquipmentSlot& rhs) const { return EquipmentSlot == rhs.EquipmentSlot; }

};