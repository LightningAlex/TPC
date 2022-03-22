// Fill out your copyright notice in the Description page of Project Settings.


#include "TPAnimInstance.h"
#include "TPCharacter.h"

/*ANIM INSTANCE PROXY*/
void FTPAnimInstanceProxy::Initialize(UAnimInstance* InAnimInstance)
{
	Super::Initialize(InAnimInstance);
	UTPAnimInstance* TPAnimInst = GetTPAnimInstance();
	if (TPAnimInst != nullptr)
	{
		OwningActorPtr = TWeakObjectPtr<AActor>(TPAnimInst->GetOwningActor());
		if (OwningActorPtr.IsValid())
		{
			CharacterOwner = Cast<ATPCharacter>(OwningActorPtr.Get());
			if (CharacterOwner != nullptr)
			{
				OwnerTPMovement = CharacterOwner->GetTPCharacterMovement();
			}
		}
	}
}

void FTPAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (CharacterOwner != nullptr)
	{
		CharacterVelocity = CharacterOwner->GetVelocity();


		RagdollEndRootRot = CharacterOwner->GetRagdollEndRootRotation();
		if (CharacterOwner->IsRecoveringFromRagdoll())
		{
			RagdollRecAlpha = CharacterOwner->GetRagdollRecoveryAlpha();
		}
		else if (CharacterOwner->IsRagdoll())
		{
			RagdollRecAlpha = 1.f;
			GetTPAnimInstance()->SaveRagdollPose();
		}
	}
}

void FTPAnimInstanceProxy::Update(float DeltaSeconds)
{
	Super::Update(DeltaSeconds);
	CharacterSpeed = CharacterVelocity.Size();
}

/*ANIM INSTANCE*/
UTPAnimInstance::UTPAnimInstance(const FObjectInitializer& OI)
	:Super(OI),
	RagdollEndPose("RagdollEndPose")
{

}

FAnimInstanceProxy* UTPAnimInstance::CreateAnimInstanceProxy()
{
	return &TPAnimInstanceProxy;
}

void UTPAnimInstance::DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy)
{

}

void UTPAnimInstance::SaveRagdollPose()
{
	SavePoseSnapshot(RagdollEndPose);
}
