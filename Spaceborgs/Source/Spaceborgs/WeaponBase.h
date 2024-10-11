// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "WeaponBase.generated.h"



class UAnimMontage;
class UNiagaraSystem;
class USoundBase;
class ABullet;
class ASpaceborgsCharacter;
class AStaticMeshComponent;


UCLASS()
class SPACEBORGS_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(VisibleAnywhere)
	bool IsPickedUp;
	UPROPERTY(VisibleAnywhere)
	bool IsEquipped;

	UPROPERTY(VisibleAnywhere)
	bool IsRifle;

	virtual void ShootRifle(float DeltaTime, 
		FHitResult OutHit, 
		class ASpaceborgsCharacter* controller,
		UAnimMontage* ShootMontage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 AmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* EmptyClipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABullet> Bullet;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttachToWeaponHolder(class ASpaceborgsCharacter* controller, UStaticMeshComponent* GunHolder);

	virtual void Shoot(FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage);

	virtual void Reload(class ASpaceborgsCharacter* controller, UAnimMontage* ReloadMontage);

	void ToggleVisibility();

	void ToggleHighlight(bool IsLookingAt);

	UFUNCTION(BlueprintCallable)
	void ChangeAnimationTree();


	UPROPERTY(VisibleAnywhere)
	float R_Timer;

	UPROPERTY(VisibleAnywhere)
	bool R_IsDelaying;


};
