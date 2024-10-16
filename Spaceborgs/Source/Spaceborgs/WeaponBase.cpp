// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/PrimitiveComponent.h"
#include "Animation/AnimInstance.h"


#include "Spaceborgs.h"
#include "Bullet.h"
#include "Components/StaticmeshComponent.h"
#include "TP_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



#include "SpaceborgsCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsPickedUp = false;
	IsEquipped = false;

	IsRifle = false;
	ReloadTime = 1.f;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

void AWeaponBase::ShootRifle(float DeltaTime, FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage)
{
	UAnimInstance* pAnimInstance = controller->GetMesh1P()->GetAnimInstance();
	if (pAnimInstance)
	{
		pAnimInstance->Montage_Play(ShootMontage);
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::AttachToWeaponHolder(class ASpaceborgsCharacter* controller, UStaticMeshComponent* GunHolder)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attach to weapon holder"));

	controller->HasWeapon = true;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	this->AttachToComponent(GunHolder, AttachmentRules);
	UE_LOG(LogTemp, Warning, TEXT("Weapon attached to socket"));

}

void AWeaponBase::Shoot(FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage)
{
	UAnimInstance* pAnimInstance = controller->GetMesh1P()->GetAnimInstance();
	if (pAnimInstance)
	{
		pAnimInstance->Montage_Play(ShootMontage);
	}
}



void AWeaponBase::Reload(class ASpaceborgsCharacter* controller, UAnimMontage* ReloadMontage)
{
	if (R_IsDelaying == false)
	{
		if (AmmoCount < MaxAmmo)
		{
			UAnimInstance* pAnimInstance = controller->GetMesh1P()->GetAnimInstance();
			if (pAnimInstance)
			{
				pAnimInstance->Montage_Play(ReloadMontage, 1.0f);
			}
			AmmoCount = MaxAmmo;
			R_IsDelaying = true;
			R_Timer = ReloadTime;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RELOADING"));
			UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
		}
	}
}


void AWeaponBase::ToggleVisibility()
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	if (Components.Num() > 0)
	{
		for (UActorComponent* Component : Components)
		{
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component);
			if (PrimitiveComponent)
			{
				if (IsEquipped)
					PrimitiveComponent->SetVisibility(true);
				if (!IsEquipped)
					PrimitiveComponent->SetVisibility(false);
			}
		}
	}

}

void AWeaponBase::ToggleHighlight(bool IsLookingAt)
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	if (Components.Num() > 0)
	{
		for (UActorComponent* Component : Components)
		{
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component);
			if (PrimitiveComponent)
			{
				if (IsLookingAt)
					PrimitiveComponent->SetRenderCustomDepth(true);
				if (!IsLookingAt)
					PrimitiveComponent->SetRenderCustomDepth(false);
			}
		}
	}
}

void AWeaponBase::ChangeAnimationTree()
{

}