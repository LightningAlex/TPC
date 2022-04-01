// Fill out your copyright notice in the Description page of Project Settings.


#include "TPWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/ShapeComponent.h"
#include "TPCombatComponent.h"
#include "TPCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "TPDeveloperSettings.h"

ATPWeaponBase::ATPWeaponBase()
	:
	Super(),
	WeaponElement(EMagicElementType::MET_NONE),
	WeaponBaseForce(100.f),
	WeaponPoiseRecoveryRate(2.f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	for (int32 i = 0; i < static_cast<int32>(EAttackType::MAX); i++)
	{
		AttackBlockMultipliers[i] = 1.f;
	}

	for (int32 i = 0; i < static_cast<int32>(EMagicElementType::MAX); i++)
	{
		MagicBlockMultipliers[i] = (i == static_cast<int32>(WeaponElement) && i != 0) ? 3.f : 1.f;
	}
}

void ATPWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetWeaponBlocking() && CurrentWeaponPoise < MaxWeaponPoise - 0.002f)
	{
		CurrentWeaponPoise = FMath::Min(CurrentWeaponPoise + WeaponPoiseRecoveryRate * DeltaTime, MaxWeaponPoise);
	}
}

void ATPWeaponBase::BeEquipped(UTPCombatComponent* InCombatComponent, FName InSlotName /*= NAME_None*/)
{
	Super::BeEquipped(InCombatComponent, InSlotName);

	for (const FWeaponCollider& ObservedCollider : WeaponColliders)
	{
		ObservedCollider.ActiveCollider->IgnoreActorWhenMoving(GetEquipmentHolder(), true);
		ObservedCollider.ActiveCollider->IgnoreActorWhenMoving(this, true);
		ObservedCollider.ActiveCollider->OnComponentBeginOverlap.AddDynamic(this, &ATPWeaponBase::OnWeaponOverlap);
	}
}

void ATPWeaponBase::SetWeaponElementType(EMagicElementType NewElement, float NewElementDuration /*= -1.f*/)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ElementHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ElementHandle);
	}

	WeaponElement = NewElement;

	if (NewElementDuration > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ElementHandle, this, &ATPWeaponBase::RestoreElementType, NewElementDuration, false);
	}
}

void ATPWeaponBase::ActivateWeapon(const TArray<FName>& CollidersToActivate, EAttackType InAttackType)
{
	for (const FWeaponCollider& ObservedCollider : WeaponColliders)
	{
		if (CollidersToActivate.Num() == 0 || CollidersToActivate.Contains(ObservedCollider.ColliderName))
		{
			ObservedCollider.ActiveCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	CurrentAttackType = InAttackType;
	bWeaponActive = true;
}

void ATPWeaponBase::DeactivateWeapon()
{
	if (bWeaponActive)
	{
		for (const FWeaponCollider& ObservedCollider : WeaponColliders)
		{
			if (ObservedCollider.ActiveCollider->IsCollisionEnabled())
			{
				ObservedCollider.ActiveCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		DamagedActors.Empty();
		bWeaponActive = false;
	}
}

void ATPWeaponBase::RestoreElementType()
{
	WeaponElement = OrgWeaponElement;
}

void ATPWeaponBase::SetWeaponBlocking(bool bNewBlocking)
{
	bWeaponBlocking = bNewBlocking;
}

bool ATPWeaponBase::WeaponTakeHit_Implementation(EAttackType InAttackType, EMagicElementType InMagicType, float InForce)
{
	check (!GetWeaponBlocking()); // This function should never be called if the weapon isn't blocking!

	const float AT_Value = AttackBlockMultipliers[static_cast<int32>(InAttackType)];
	const float MT_Value = MagicBlockMultipliers[static_cast<int32>(InMagicType)];

	if (AT_Value == 0.f || MT_Value == 0.f)
	{
		return false;
	}

	InForce /= (AT_Value * MT_Value);

	CurrentWeaponPoise = FMath::Max(0.f, CurrentWeaponPoise - InForce);
	return CurrentWeaponPoise > 0.f;
}

void ATPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	OrgWeaponElement = WeaponElement;
	CurrentWeaponPoise = MaxWeaponPoise;
}

void ATPWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ElementHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ElementHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void ATPWeaponBase::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamagedActors.Contains(OtherActor) || OtherActor == GetEquipmentHolder())
	{
		return;
	}

	float FinalForceMultiplier = 1.f;

	const UTPDeveloperSettings* TPDS = GetDefault<UTPDeveloperSettings>();
	if (TPDS)
	{
		GetEquipmentHolder()->GetMesh()->GetAnimInstance()->GetCurveValue(TPDS->WeaponForceCurveName, FinalForceMultiplier);
	}
	const float FinalForce = WeaponBaseForce * FinalForceMultiplier;

	ATPWeaponBase* HitWeapon = Cast<ATPWeaponBase>(OtherActor);
	if (HitWeapon && !DamagedActors.Contains(HitWeapon->GetEquipmentHolder()) && HitWeapon->GetWeaponBlocking())
	{
		if (HitWeapon->WeaponTakeHit(CurrentAttackType, WeaponElement, FinalForce))
		{
			GetEquipmentHolder()->TPCombatComponent->ParryAttack(GetAttachedToSlot(), WeaponType);
			return;
		}
		else
		{
			HitWeapon->GetEquipmentHolder()->TPCombatComponent->BreakBlock(HitWeapon->GetAttachedToSlot(), HitWeapon->WeaponType);
		}
	}
	else if (ATPCharacter* HitChar = Cast<ATPCharacter>(OtherActor))
	{
		FVector HitDir = (SweepResult.ImpactPoint - OverlappedComponent->GetComponentLocation()).GetSafeNormal(0.001f);
		HitChar->TPCombatComponent->TakeHit(GetEquipmentHolder()->TPCombatComponent->GetFullValue(ECombatValue::ECV_ATTACK), FinalForce,
			HitDir, CurrentAttackType, WeaponElement, SweepResult);
	}
	else
	{
		/*This needs reworking!*/
		FVector HitFromDir = (OtherComp->GetComponentLocation() - OverlappedComponent->GetComponentLocation()).GetSafeNormal();
		OtherComp->AddImpulse(HitFromDir * FinalForce, NAME_None, true);
		UGameplayStatics::ApplyPointDamage(OtherActor, GetEquipmentHolder()->TPCombatComponent->GetFullValue(ECombatValue::ECV_ATTACK), 
			HitFromDir, SweepResult, GetEquipmentHolder()->GetController(), this, UDamageType::StaticClass());
	}
}