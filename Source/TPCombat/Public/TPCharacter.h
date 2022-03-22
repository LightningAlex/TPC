// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPRagdollDefaults.h"
#include "TPWeaponDefaults.h"
#include "TPCharacter.generated.h"

class UTPCharacterMovementComponent;
class UTPCombatComponent;
class UCurveFloat;
class UAnimMontage;
class UTPAbility;

UCLASS()
class TPCOMBAT_API ATPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTPCombatComponent* TPCombatComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	UAnimMontage* GetUpMontages[ERagdollDirection::MAX];
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	UCurveFloat* RagdollAlphaCurves[ERagdollDirection::MAX];
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	FName RootBoneName;
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	FName RagdollFaceBoneName;
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	FName SkeletonRootChild;
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	EActualForwardVector MeshForwardVector;
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	EActualForwardVector SkeletonRootChildActualForward;
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	bool bMeshForwardInverted;
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	bool bSkeleteonRootChildInverted;
	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	bool bDisableRecovery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UTPAbility>> CharacterAbilities;

private:
	/*Ragdoll*/
	bool bRagdolling;
	bool bRecoveringFromRagdoll;
	float RagdollDuration;
	float RagETime;
	float RagdollRecoveryAlpha;
	float RagRecoveryETime;
	FRotator RagdollEndRootRotation;
	FVector OrgMeshRelativeLoc;
	FVector MeshRecoveryStartLoc;
	FRotator OrgMeshRelativeRot;
	ERagdollDirection CurrentRecoveryDirection;

public:
	ATPCharacter(const FObjectInitializer& OI);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	UTPCharacterMovementComponent* GetTPCharacterMovement();

	/*Ragdoll functions*/
	UFUNCTION(BlueprintCallable)
	void StartRagdoll(float InDuration, const FVector& InImpulseVector, const FName& InImpulseBone);
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsRagdoll() const { return bRagdolling; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsRecoveringFromRagdoll() const { return bRecoveringFromRagdoll; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE FRotator GetRagdollEndRootRotation() const { return RagdollEndRootRotation; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetRagdollRecoveryAlpha() const { return RagdollRecoveryAlpha; }

	/*Montage getters*/
	UFUNCTION(BlueprintNativeEvent)
	UAnimMontage* GetParryMontage(const FName& ParriedSocketName, EWeaponType InWT);
	UFUNCTION(BlueprintNativeEvent)
	UAnimMontage* GetBlockBreakMontage(const FName& BrokenSocketName, EWeaponType InWT);

	/*Movement functions*/
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveMovementInput(const FVector& MovementVector);

protected:
	virtual void BeginPlay() override;

private:
	void EndRagdoll();
	ERagdollDirection GetRagdollRecoverDirection(const FQuat& EndRootRot);
	float GetLowestSocketLocation();
};
