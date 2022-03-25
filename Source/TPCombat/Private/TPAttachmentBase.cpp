#include "TPAttachmentBase.h"
#include "Components/StaticMeshComponent.h"

ATPAttachmentBase::ATPAttachmentBase()
	:Super()
{
	AttachmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachmentMesh"));
	AttachmentMesh->SetupAttachment(GetRootComponent());
}