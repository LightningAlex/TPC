// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacter.h"
#include "TPCharacterMovementComponent.h"
#include "TPCombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"
#include "TPAnimInstance.h"

ATPCharacter::ATPCharacter(const FObjectInitializer& OI)
	:Super(OI.SetDefaultSubobjectClass<UTPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)),
	RootBoneName("root"),
	RagdollFaceBoneName("pelvis"),
	SkeletonRootChild("pelvis"),
	MeshForwardVector(EActualForwardVector::AFV_Y),
	SkeletonRootChildActualForward(EActualForwardVector::AFV_Y)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TPCombatComponent = CreateDefaultSubobject<UTPCombatComponent>(TEXT("TPCombatComponent"));
}

void ATPCharacter::BeginPlay()
{
	Super::BeginPlay();

	OrgMeshRelativeLoc = GetMesh()->GetRelativeLocation();
	OrgMeshRelativeRot = GetMesh()->GetRelativeRotation();
}

void ATPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRagdoll())
	{
		FVector RagdollLoc = GetMesh()->GetSocketLocation(RootBoneName);
		RagdollLoc.Z = GetLowestSocketLocation() + GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FRotator RagdollRot = GetActorRotation();
		FTransform SkeletonRootTrans = GetMesh()->GetSocketTransform(SkeletonRootChild);
		FVector RotVec = SkeletonRootTrans.GetLocation() - GetMesh()->GetSocketLocation(RootBoneName);
		/*Check if flipped*/
		FVector UsedForDot = FVector::ZeroVector;
		switch (SkeletonRootChildActualForward)
		{
		case EActualForwardVector::AFV_X:
			UsedForDot = SkeletonRootTrans.GetRotation().GetForwardVector();
			break;
		case EActualForwardVector::AFV_Y:
			UsedForDot = SkeletonRootTrans.GetRotation().GetRightVector();
			break;
		case EActualForwardVector::AFV_Z:
			UsedForDot = SkeletonRootTrans.GetRotation().GetUpVector();
			break;
		}
		if (bSkeleteonRootChildInverted)
		{
			UsedForDot *= -1.f;
		}
		if (FVector::DotProduct(UsedForDot, GetActorUpVector()) > 0.f)
		{
			RotVec *= -1.f;
		}
		RagdollRot.Yaw = FRotationMatrix::MakeFromX(RotVec).Rotator().Yaw;
		SetActorLocationAndRotation(FMath::VInterpTo(GetActorLocation(), RagdollLoc, DeltaTime, 5.f), RagdollRot);

		FQuat RootBoneQuat = GetMesh()->GetSocketTransform(RootBoneName).GetRotation();
		RagdollEndRootRotation = FRotationMatrix::MakeFromXY(RootBoneQuat.GetForwardVector(), RootBoneQuat.GetRightVector()).ToQuat().Rotator();

		if (!GetCharacterMovement()->IsFalling())
		{
			RagETime += DeltaTime;
			if (RagETime > RagdollDuration)
			{
				EndRagdoll();
			}
		}
	}
	else if (IsRecoveringFromRagdoll())
	{
		RagdollRecoveryAlpha = RagdollAlphaCurves[static_cast<int32>(CurrentRecoveryDirection)]->GetFloatValue(RagRecoveryETime);
		GetMesh()->SetRelativeLocation(FMath::Lerp<FVector>(MeshRecoveryStartLoc, OrgMeshRelativeLoc, 1.f - RagdollRecoveryAlpha));
		/*#TODO Make capsule move towards mesh, not vice-versa!*/
		float MinCurveTime = 0.f;
		float MaxCurveTime = 0.f;
		RagdollAlphaCurves[static_cast<int32>(CurrentRecoveryDirection)]->GetTimeRange(MinCurveTime, MaxCurveTime);
		RagRecoveryETime += DeltaTime;
		if (RagRecoveryETime > MaxCurveTime)
		{
			bRecoveringFromRagdoll = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UTPCharacterMovementComponent* ATPCharacter::GetTPCharacterMovement()
{
	return Cast<UTPCharacterMovementComponent>(GetCharacterMovement());
}

void ATPCharacter::StartRagdoll(float InDuration, const FVector& InImpulseVector, const FName& InImpulseBone)
{
	UE_LOG(LogTemp, Log, TEXT("Duration: %f"), InDuration);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	FVector FinalImpulse = InImpulseVector * 100.f;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->AddImpulse(FinalImpulse, InImpulseBone);
	RagdollDuration = InDuration;
	RagETime = 0.f;
	RagdollRecoveryAlpha = 1.f;
	bRagdolling = true;
}

UAnimMontage* ATPCharacter::GetParryMontage_Implementation(TSubclassOf<UTPAbility> ParriedAbility)
{
	return nullptr;
}

UAnimMontage* ATPCharacter::GetBlockBreakMontage_Implementation(const FName& BrokenSocketName, EWeaponType InWT)
{
	return nullptr;
}

void ATPCharacter::EndRagdoll()
{

	CurrentRecoveryDirection = GetRagdollRecoverDirection(RagdollEndRootRotation.Quaternion());

	//RagdollRecoveryAlpha = 1.f;

	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, false));
	GetMesh()->SetRelativeRotation(OrgMeshRelativeRot);
	if (CurrentRecoveryDirection != ERagdollDirection::RD_NONE)
	{
		UE_LOG(LogTemp, Log, TEXT("Animation playing!"));
		GetMesh()->GetAnimInstance()->Montage_Play(GetUpMontages[static_cast<int32>(CurrentRecoveryDirection)]);
	}
	MeshRecoveryStartLoc = GetMesh()->GetRelativeLocation();
	RagRecoveryETime = 0.f;
	if (!bDisableRecovery)
	{
		bRecoveringFromRagdoll = true;
	}
	bRagdolling = false;
}

ERagdollDirection ATPCharacter::GetRagdollRecoverDirection(const FQuat& EndRootRot)
{
	float RootUpDot = FVector::DotProduct(EndRootRot.GetUpVector(), GetActorUpVector());
	if (RootUpDot > 0.5f)
	{
		return ERagdollDirection::RD_NONE;
	}
	FVector ActualMeshForward = FVector::ZeroVector;
	switch (MeshForwardVector)
	{
	case EActualForwardVector::AFV_X:
		ActualMeshForward = GetMesh()->GetForwardVector();
		break;
	case EActualForwardVector::AFV_Y:
		ActualMeshForward = GetMesh()->GetRightVector();
		break;
	case EActualForwardVector::AFV_Z:
		ActualMeshForward = GetMesh()->GetUpVector();
		break;
	}
	if (bMeshForwardInverted)
	{
		ActualMeshForward *= -1.f;
	}
	if (FVector::DotProduct(ActualMeshForward, GetActorUpVector()) > 0.f)
	{
		return ERagdollDirection::RD_BACKWARD;
	}
	return ERagdollDirection::RD_FORWARD;
}

float ATPCharacter::GetLowestSocketLocation()
{
	float NewLowest = BIG_NUMBER;
	for (const FName& ObservedSocket : GetMesh()->GetAllSocketNames())
	{
		if (ObservedSocket != RootBoneName && GetMesh()->IsBodyGravityEnabled(ObservedSocket))
		{
			float ObservedSocketZ = GetMesh()->GetSocketLocation(ObservedSocket).Z;
			if (ObservedSocketZ < NewLowest)
			{
				NewLowest = ObservedSocketZ;
			}
		}
	}
	return NewLowest;
}

void ATPCharacter::ReceiveMovementInput(const FVector& MovementVector)
{
	if (!IsRagdoll() && !IsRecoveringFromRagdoll())
	{
		AddMovementInput(MovementVector);
	}
}