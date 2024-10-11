// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifle.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"

AAssaultRifle::AAssaultRifle()
{
	Damage = 25.0f;
	Range = 1000.0f;
	AmmoCount = 30;
	MaxAmmo = 30;
	ReloadTime = 2.0f;
	FireRate = 0.2f;
	Timer = FireRate;
	MuzzleFlash = nullptr;
	FireSound = nullptr;
	ReloadSound = nullptr;
	ReloadTime = 1.f;

	IsRifle = true;
}

void AAssaultRifle::BeginPlay()
{
	Super::BeginPlay();
	R_Timer = ReloadTime;
	R_IsDelaying = false;
	MuzzleLocation = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("MuzzlePoint")));

}

void AAssaultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (R_IsDelaying)
	{
		R_Timer -= DeltaTime;

		if (R_Timer <= 0)
		{
			R_IsDelaying = false;
			R_Timer = 0; // Optional: Reset the timer to 0
		}
	}

	if (IsDelaying)
	{
		Timer += DeltaTime;
		if (Timer > FireRate)
		{
			IsDelaying = false;
		}
	}
}


void AAssaultRifle::ShootRifle(float DeltaTime, FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage)
{
	if (R_IsDelaying == false && IsDelaying == false)
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
				//WHAT IT DO HERE
				AmmoCount--;

				Super::ShootRifle(DeltaTime, OutHit, controller, ShootMontage);
				FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
				FVector AimPoint = OutHit.bBlockingHit ? OutHit.Location : OutHit.TraceEnd;
				FVector ShootDirection = (AimPoint - MuzzlePos).GetSafeNormal();
				
				//FRotator MuzzleRotation = MuzzleLocation->GetComponentRotation();

				
				
				UWorld* World = GetWorld();

				if (World != nullptr)
				{
					if (MuzzleFlash) // Assuming MuzzleFlashFX is a UNiagaraSystem*
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlash, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation());
					}
					World->SpawnActor<ABullet>(Bullet, MuzzlePos, UKismetMathLibrary::MakeRotFromX(ShootDirection));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BULLET HAS BEEN SHOT"));
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
}