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

ATPWeaponBase::ATPWeaponBase()
	:
	Super(),
	WeaponElement(EMagicElementType::MET_NONE),
	WeaponForce(100.f),
	WeaponPoiseRecoveryRate(2.f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void ATPWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetWeaponBlocking() && CurrentWeaponPoise < MaxWeaponPoise - 0.002f)
	{
		CurrentWeaponPoise = FMath::Min(CurrentWeaponPoise + WeaponPoiseRecoveryRate * DeltaTime, MaxWeaponPoise);
	}
}

void ATPWeaponBase::BeEquipped(UTPCombatComponent* InCombatComponent, FName InSocketName /* = NAME_None */)
{
	Super::BeEquipped(InCombatComponent, InSocketName);

	for (const FWeaponCollider& ObservedCollider : WeaponColliders)
	{
		ObservedCollider.ActiveCollider->IgnoreActorWhenMoving(GetEquipmentHolder(), true);
		ObservedCollider.ActiveCollider->IgnoreActorWhenMoving(this, true);
		ObservedCollider.ActiveCollider->OnComponentBeginOverlap.AddDynamic(this, &ATPWeaponBase::OnWeaponOverlap);
	}
}

void ATPWeaponBase::SetWeaponElementType(EMagicElementType NewElement, float NewElementDuration /*= -1.f*/)
{
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

bool ATPWeaponBase::WeaponTakeHit(float InForce)
{
	check (!GetWeaponBlocking()); // This function should never be called if the weapon isn't blocking!

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
	ATPWeaponBase* HitWeapon = Cast<ATPWeaponBase>(OtherActor);
	if (HitWeapon && !DamagedActors.Contains(HitWeapon->GetEquipmentHolder()) && HitWeapon->GetWeaponBlocking())
	{
		if (HitWeapon->WeaponTakeHit(WeaponForce))
		{
			GetEquipmentHolder()->TPCombatComponent->ParryAttack(GetAttachedToSocket(), WeaponType);
			return;
		}
		else
		{
			HitWeapon->GetEquipmentHolder()->TPCombatComponent->BreakBlock(HitWeapon->GetAttachedToSocket(), HitWeapon->WeaponType);
		}
	}
	else if (ATPCharacter* HitChar = Cast<ATPCharacter>(OtherActor))
	{
		FVector HitDir = (SweepResult.ImpactPoint - OverlappedComponent->GetComponentLocation()).GetSafeNormal(0.001f);
		HitChar->TPCombatComponent->TakeHit(GetEquipmentHolder()->TPCombatComponent->GetFullValue(ECombatValue::ECV_ATTACK), WeaponForce,
			HitDir, CurrentAttackType, WeaponElement, SweepResult);
	}
	else
	{
		/*This needs reworking!*/
		FVector HitFromDir = (OtherComp->GetComponentLocation() - OverlappedComponent->GetComponentLocation()).GetSafeNormal();
		OtherComp->AddImpulse(HitFromDir * WeaponForce, NAME_None, true);
		UGameplayStatics::ApplyPointDamage(OtherActor, GetEquipmentHolder()->TPCombatComponent->GetFullValue(ECombatValue::ECV_ATTACK), 
			HitFromDir, SweepResult, GetEquipmentHolder()->GetController(), this, UDamageType::StaticClass());
	}
}