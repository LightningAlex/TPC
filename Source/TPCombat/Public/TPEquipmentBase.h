// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPCombatValues.h"
#include "TPEquipmentBase.generated.h"

class UTPCombatComponent;
class USceneComponent;
class ATPCharacter;

UCLASS(Abstract, NotBlueprintable)
class TPCOMBAT_API ATPEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* EquipmentRoot;

public:
	UPROPERTY(EditDefaultsOnly)
	float AddedValues[ECombatValue::MAX];

private:
	FName AttachedToSocket;
	UPROPERTY()
	ATPCharacter* EquipmentHolder;

public:	
	ATPEquipmentBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void BeEquipped(UTPCombatComponent* InCombatComponent, FName InSocketName = NAME_None);
	UFUNCTION(BlueprintCallable)
	virtual void BeUnequipped(UTPCombatComponent* InCombatComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void Event_Equipped(UTPCombatComponent* InCombatComponent, FName InSocketName = NAME_None);
	UFUNCTION(BlueprintImplementableEvent)
	void Event_Unequipped(UTPCombatComponent* InCombatComponent);

	UFUNCTION(BlueprintPure)
	float GetAddedValue(ECombatValue SearchedValue);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetAttachedToSocket() const { return AttachedToSocket; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE ATPCharacter* GetEquipmentHolder() const { return EquipmentHolder; }

protected:
	virtual void BeginPlay() override;


};
