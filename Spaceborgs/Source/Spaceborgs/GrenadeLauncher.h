// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class SPACEBORGS_API AGrenadeLauncher : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AGrenadeLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	void Shoot(FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage) override;


private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MuzzleLocation;

	UPROPERTY(VisibleAnywhere)
	float Timer;

	UPROPERTY()
	bool IsDelaying;

	UPROPERTY()
	AActor* ParentActor;
};
