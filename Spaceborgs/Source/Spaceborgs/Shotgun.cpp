// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

AShotgun::AShotgun()
{
	Damage = 25.0f;
	Range = 1000.0f;
	MaxAmmo = 12;
	AmmoCount = MaxAmmo;
	ReloadTime = 2.0f;
	FireRate = 1.f;
	Timer = FireRate;
	IsDelaying = false;
	MuzzleFlash = nullptr;
	FireSound = nullptr;
	ReloadSound = nullptr;

}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();

	MuzzleLocation = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("MuzzlePoint")));

}

void AShotgun::Tick(float DeltaTime)
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
}


void AShotgun::Shoot()
{
	if (Timer >= FireRate)
	{
		//WHAT IT DO HERE
		AmmoCount--;

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
			FVector MuzzlePos = MuzzleLocation->GetComponentLocation();

			FRotator MuzzleRotation = MuzzleLocation->GetComponentRotation();
			UWorld* World = GetWorld();

			if (World != nullptr)
			{
				for (int i = 0; i < 5; i++)
				{
					FRotator RandomRot = MuzzleRotation;
					RandomRot.Yaw += FMath::RandRange(-10.0f, 10.0f);
					RandomRot.Pitch += FMath::RandRange(-5.0f, 5.0f);

					World->SpawnActor<ABullet>(Bullet, MuzzlePos, RandomRot);

					Timer = 0;
					IsDelaying = true;
				}
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BULLET HAS BEEN SHOT"));
			}
		}
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}

}

void AShotgun::Reload()
{
	//AmmoCount + 12;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RELOADING"));
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}
