// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacterMovementComponent.h"

void UTPCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OrgMovementSpeed = MaxWalkSpeed;
}

void UTPCharacterMovementComponent::SetMaxMovementMultiplier(float InMultiplier)
{
	MaxWalkSpeed = OrgMovementSpeed * InMultiplier;
}

void UTPCharacterMovementComponent::RestoreOriginalWalkingSpeed()
{
	MaxWalkSpeed = OrgMovementSpeed;
}