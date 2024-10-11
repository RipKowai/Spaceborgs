// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Pistol.generated.h"
/**
 *
 */
UCLASS()
class SPACEBORGS_API APistol : public AWeaponBase
{
	GENERATED_BODY()

public:
	APistol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	void Shoot(FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage) override;



private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MuzzleLocation;

};

