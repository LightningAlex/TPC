// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "TPAnimInstance.generated.h"

class ATPCharacter;
class UTPAnimInstance;
class UTPCharacterMovementComponent;

USTRUCT(BlueprintType)
struct FTPAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	FTPAnimInstanceProxy()
	{
	}

public:
	UPROPERTY(BlueprintReadOnly)
	ATPCharacter* CharacterOwner;
	UPROPERTY()
	TWeakObjectPtr<AActor> OwningActorPtr;
	UPROPERTY(BlueprintReadOnly)
	UTPCharacterMovementComponent* OwnerTPMovement;
	UPROPERTY(BlueprintReadOnly)
	float CharacterSpeed;
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity;
	UPROPERTY(BlueprintReadOnly)
	FRotator RagdollEndRootRot;
	UPROPERTY(BlueprintReadOnly)
	float RagdollRecAlpha;

public:
	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;

	FORCEINLINE UTPAnimInstance* GetTPAnimInstance()
	{
		return Cast<UTPAnimInstance>(GetAnimInstanceObject());
	}
};

UCLASS()
class TPCOMBAT_API UTPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FTPAnimInstanceProxy TPAnimInstanceProxy;

protected:
	UPROPERTY(BlueprintReadOnly)
	FName RagdollEndPose;

public:
	UTPAnimInstance(const FObjectInitializer& OI);

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;
	void SaveRagdollPose();

};
