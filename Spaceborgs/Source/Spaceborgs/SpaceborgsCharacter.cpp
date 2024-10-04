// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceborgsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpaceborgsProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "WeaponBase.h"
#include "Engine/LocalPlayer.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASpaceborgsCharacter

ASpaceborgsCharacter::ASpaceborgsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetSphereRadius(150.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASpaceborgsCharacter::HandleBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ASpaceborgsCharacter::HandleEndOverlap);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	IsLookingAtWeapon = false;
	IsCloseToWeapon = false;

}

void ASpaceborgsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	SlotHolder = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("GunHolder")));
}

void ASpaceborgsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsCloseToWeapon)
		HandleRaycast(DeltaTime);

}

void ASpaceborgsCharacter::HandleRaycast(float DeltaTime)
{
	FHitResult OutHit;

	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = (Start + (ForwardVector * 1000.f));

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);


	if (isHit)
	{
		if (OutHit.bBlockingHit)
		{
			AWeaponBase* Weapon = Cast<AWeaponBase>(OutHit.GetActor());
			if (OutHit.GetActor()->IsA<AWeaponBase>() && WeaponsCloseTo.Contains(Weapon))
			{
				Target = OutHit.GetActor();
				SetLookingAtWeapon(true);
			}
			else
			{
				SetLookingAtWeapon(false);
			}
		}
		else
		{
			SetLookingAtWeapon(false);
		}
	}
	else
	{
		SetLookingAtWeapon(false);
	}
}

void ASpaceborgsCharacter::SetLookingAtWeapon(bool IsLooking)
{
	IsLookingAtWeapon = IsLooking;
	AWeaponBase* Weapon = Cast<AWeaponBase>(Target);
	if (Weapon)
		Weapon->ToggleHighlight(IsLookingAtWeapon);
}



void ASpaceborgsCharacter::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AWeaponBase>())
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(OtherActor);
		if (!Weapon->IsPickedUp)
		{
			IsCloseToWeapon = true;
			WeaponsCloseTo.Add(Weapon);
		}

	}
}

void ASpaceborgsCharacter::HandleEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AWeaponBase>())
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(OtherActor);
		Weapon->ToggleHighlight(false);
		WeaponsCloseTo.Remove(Weapon);
		if (WeaponsCloseTo.Num() == 0)
			IsCloseToWeapon = false;
	}
	if (OtherActor == Target)
	{
		Target = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ASpaceborgsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpaceborgsCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpaceborgsCharacter::Look);

		// Pick up
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &ASpaceborgsCharacter::PickUp);

		// Switch Weapon
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ASpaceborgsCharacter::SwitchWeapon);


		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASpaceborgsCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASpaceborgsCharacter::StopSprinting);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASpaceborgsCharacter::Fire);
		//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ASpaceborgsCharacter::StopFire);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ASpaceborgsCharacter::Reload);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ASpaceborgsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASpaceborgsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASpaceborgsCharacter::PickUp(const FInputActionValue& Value)
{
	if (Target != nullptr && WeaponsCloseTo.Contains(Target) && IsLookingAtWeapon)
	{
		AWeaponBase* PickedUpWeapon = Cast<AWeaponBase>(Target);
		if (PickedUpWeapon)
		{
			if (!PickedUpWeapon->IsPickedUp)
			{
				PickedUpWeapon->AttachToWeaponHolder(this, SlotHolder);
				WeaponInventory.Add(PickedUpWeapon);

				PickedUpWeapon->IsPickedUp = true;
				PickedUpWeapon->IsEquipped = true;
				PickedUpWeapon->ToggleVisibility();
			}

			for (AWeaponBase* Weapon : WeaponInventory)
			{
				if (Weapon != PickedUpWeapon)
				{
					Weapon->IsEquipped = false;
					Weapon->ToggleVisibility();
				}
			}

			CurrentWeaponIndex = WeaponInventory.Num() - 1;
			PickedUpWeapon->ToggleHighlight(false);
			CurrentWeapon = PickedUpWeapon;
			WeaponsCloseTo.Remove(PickedUpWeapon);
			if (WeaponsCloseTo.Num() == 0)
				IsCloseToWeapon = false;
			Target = nullptr;
		}
	}
}

void ASpaceborgsCharacter::SwitchWeapon(const FInputActionValue& Value)
{
	if (WeaponInventory.Num() > 0)
	{
		CurrentWeaponIndex += (int)Value.Get<float>();

		if (CurrentWeaponIndex > WeaponInventory.Num() - 1)
			CurrentWeaponIndex = 0;
		else if (CurrentWeaponIndex < 0)
			CurrentWeaponIndex = WeaponInventory.Num() - 1;

		CurrentWeapon = WeaponInventory[CurrentWeaponIndex];

		for (AWeaponBase* Weapon : WeaponInventory)
		{
			if (Weapon != CurrentWeapon)
			{
				Weapon->IsEquipped = false;
				Weapon->ToggleVisibility();
			}
		}

		CurrentWeapon->IsEquipped = true;
		CurrentWeapon->ToggleVisibility();
	}

}

void ASpaceborgsCharacter::Sprint(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = 900.f;
	}
}

void ASpaceborgsCharacter::StopSprinting(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void ASpaceborgsCharacter::Fire(const FInputActionValue& Value)
{
	if (CurrentWeapon)
		CurrentWeapon->Shoot();
}

void ASpaceborgsCharacter::StopFire(const FInputActionValue& Value)
{

}

void ASpaceborgsCharacter::Reload(const FInputActionValue& Value)
{
	if (CurrentWeapon)
		CurrentWeapon->Reload();
}


