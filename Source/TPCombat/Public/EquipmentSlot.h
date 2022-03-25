#pragma once
#include "CoreMinimal.h"
#include "EquipmentSlot.generated.h"

class ATPEquipmentBase;

USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY()

public:
	FEquipmentSlot()
	{
	}

	FEquipmentSlot(const FName& InSocketName)
	{
		EquipmentSocket = InSocketName;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipmentSocket;
	UPROPERTY()
	ATPEquipmentBase* EquipmentItem;

public:
	/*Currently not used anywhere, as we're not comparing slots to slots*/
	bool operator == (const FEquipmentSlot& rhs) const { return EquipmentSocket == rhs.EquipmentSocket; }

};