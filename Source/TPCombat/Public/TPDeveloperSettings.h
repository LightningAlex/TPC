// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TPDeveloperSettings.generated.h"

UCLASS(config=Engine, defaultconfig, meta=(DisplayName="TP Combat Settings"), BlueprintType)
class TPCOMBAT_API UTPDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Config)
	FName WeaponForceCurveName;
	UPROPERTY(EditAnywhere, Config)
	FName AbilitySpeedCurveName;
};
