// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadeLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"


AGrenadeLauncher::AGrenadeLauncher()
{
	Damage = 25.0f;
	Range = 1000.0f;
	MaxAmmo = 10;
	AmmoCount = 10;
	ReloadTime = 2.0f;
	FireRate = 2.f;
	Timer = FireRate;
	IsDelaying = false;
	MuzzleFlash = nullptr;
	FireSound = nullptr;
	ReloadSound = nullptr;
}

void AGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();

	MuzzleLocation = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("MuzzlePoint")));

}

void AGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDelaying)
	{
		Timer += DeltaTime;
		if (Timer > FireRate)
		{
			IsDelaying = false;
		}
	}
	if (R_IsDelaying)
	{
		R_Timer -= DeltaTime;

		if (R_Timer <= 0)
		{
			R_IsDelaying = false;
			R_Timer = 0; // Optional: Reset the timer to 0
		}
	}
}


void AGrenadeLauncher::Shoot(FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage)
{
	if (Timer >= FireRate)
	{
		if (AmmoCount <= 0)
		{
			// Play empty clip sound
			if (EmptyClipSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, EmptyClipSound, GetActorLocation());
			}
			return; // Exit the function if no ammo
		}

		if (Bullet != nullptr && MuzzleLocation)
		{
			Super::Shoot(OutHit, controller, ShootMontage);

			//WHAT IT DO HERE
			AmmoCount--;

			FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
			FVector AimPoint = OutHit.bBlockingHit ? OutHit.Location : OutHit.TraceEnd;
			FVector ShootDirection = (AimPoint - MuzzlePos).GetSafeNormal();

			UWorld* World = GetWorld();

			if (World != nullptr)
			{
				World->SpawnActor<ABullet>(Bullet, MuzzlePos, UKismetMathLibrary::MakeRotFromX(ShootDirection));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GRENADE HAS BEEN SHOT"));
				Timer = 0;
				IsDelaying = true;
			}
		}
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}

}