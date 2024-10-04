// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "SpaceborgsCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsPickedUp = false;
	IsEquipped = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::AttachToWeaponHolder(class ASpaceborgsCharacter* controller, UStaticMeshComponent* GunHolder)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attach to weapon holder"));

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	this->AttachToComponent(GunHolder, AttachmentRules);
	UE_LOG(LogTemp, Warning, TEXT("Weapon attached to socket"));

}

void AWeaponBase::Shoot()
{

}
void AWeaponBase::Reload()
{

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
