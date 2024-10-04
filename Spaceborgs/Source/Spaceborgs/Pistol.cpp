// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol()
{
	Damage = 25.0f;
	Range = 1000.0f;
	AmmoCount = 12;
	MaxAmmo = 12;
	ReloadTime = 2.0f;
	FireRate = 0.5f;
	MuzzleFlash = nullptr;
	FireSound = nullptr;
	ReloadSound = nullptr;

}

void APistol::BeginPlay()
{
	Super::BeginPlay();

	MuzzleLocation = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("MuzzlePoint")));

}

void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APistol::Shoot()
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
			World->SpawnActor<ABullet>(Bullet, MuzzlePos, MuzzleRotation);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BULLET HAS BEEN SHOT"));
		}
	}
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void APistol::Reload()
{
	//AmmoCount + 12;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RELOADING"));
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}
