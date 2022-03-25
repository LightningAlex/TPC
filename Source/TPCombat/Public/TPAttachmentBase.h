#pragma once
#include "CoreMinimal.h"
#include "TPEquipmentBase.h"
#include "TPAttachmentBase.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class TPCOMBAT_API ATPAttachmentBase : public ATPEquipmentBase
{
	GENERATED_BODY()

public:
	ATPAttachmentBase();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AttachmentMesh;
};