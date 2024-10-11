// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "AssaultRifle.generated.h"
/**
 *
 */
UCLASS()
class SPACEBORGS_API AAssaultRifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	AAssaultRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	void ShootRifle(float DeltaTime, FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage) override;


private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MuzzleLocation;

	UPROPERTY(VisibleAnywhere)
	float Timer;

	UPROPERTY(VisibleAnywhere)
	bool IsDelaying;

};

