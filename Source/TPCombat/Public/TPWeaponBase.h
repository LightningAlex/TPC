// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPEquipmentBase.h"
#include "TPAttackValues.h"
#include "TPWeaponDefaults.h"
#include "TPWeaponBase.generated.h"

class UStaticMeshComponent;
class UShapeComponent;
class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct FWeaponCollider
{
	GENERATED_BODY()

public:
	FWeaponCollider()
	{
	}

public:
	UPROPERTY(BlueprintReadWrite)
	FName ColliderName;
	UPROPERTY(BlueprintReadWrite)
	UShapeComponent* ActiveCollider;
};

UCLASS(Blueprintable)
class TPCOMBAT_API ATPWeaponBase : public ATPEquipmentBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	/*Must be set in the constructor*/
	UPROPERTY(BlueprintReadWrite)
	TArray<FWeaponCollider> WeaponColliders;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMagicElementType WeaponElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponForce;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxWeaponPoise;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponPoiseRecoveryRate;

private:
	EAttackType CurrentAttackType;
	EMagicElementType OrgWeaponElement;
	FTimerHandle ElementHandle;
	float CurrentWeaponPoise;
	bool bWeaponBlocking;
	bool bWeaponActive;

	TArray<AActor*> DamagedActors;

public:
	ATPWeaponBase();
	virtual void Tick(float DeltaTime) override;
	virtual void BeEquipped(UTPCombatComponent* InCombatComponent, FName InSlotName = NAME_None) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponElementType(EMagicElementType NewElement, float NewElementDuration = -1.f);

	UFUNCTION(BlueprintCallable)
	void ActivateWeapon(const TArray<FName>& CollidersToActivate, EAttackType InAttackType);
	UFUNCTION(BlueprintCallable)
	void DeactivateWeapon();

	UFUNCTION(BlueprintCallable)
	void RestoreElementType();

	UFUNCTION(BlueprintCallable)
	void SetWeaponBlocking(bool bNewBlocking);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetWeaponBlocking() const { return bWeaponBlocking; }

	bool WeaponTakeHit(float InForce);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
