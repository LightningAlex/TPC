// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TPCOMBAT_API UTPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	float OrgMovementSpeed;

public:
	UFUNCTION(BlueprintCallable)
	void SetMaxMovementMultiplier(float InMultiplier);
	UFUNCTION(BlueprintCallable)
	void RestoreOriginalWalkingSpeed();

protected:
	virtual void BeginPlay() override;
};
