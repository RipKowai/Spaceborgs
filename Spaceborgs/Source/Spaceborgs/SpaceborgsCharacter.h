// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Containers/Array.h"
#include "SpaceborgsCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class USphereComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class ASpaceborgsCharacter : public ACharacter
{
	GENERATED_BODY()

	// Sphere that checks if in reach of weapons
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Pick Up Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickUpAction;

	/** Pick Up Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeaponAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

public:
	ASpaceborgsCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	void HandleRaycast(float DeltaTime);

	void SetLookingAtWeapon(bool IsLooking);

	UFUNCTION()
	virtual void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for PickUp input */
	void PickUp(const FInputActionValue& Value);

	/** Called for SwitchWeapon input */
	void SwitchWeapon(const FInputActionValue& Value);

	/** Called for looking input */
	void Sprint(const FInputActionValue& Value);

	/** Called for looking input */
	void StopSprinting(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);
	void StopFire(const FInputActionValue& Value);

	void Reload(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(EditAnywhere)
	AActor* Target = nullptr;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SlotHolder;

	UPROPERTY(VisibleAnywhere)
	TArray<class AWeaponBase*> WeaponInventory;

	UPROPERTY(VisibleAnywhere)
	AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere)
	int CurrentWeaponIndex;

	UPROPERTY(VisibleAnywhere)
	bool IsLookingAtWeapon;

	UPROPERTY(VisibleAnywhere)
	bool IsCloseToWeapon;

	UPROPERTY(VisibleAnywhere)
	TArray<class AWeaponBase*> WeaponsCloseTo;


public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};



