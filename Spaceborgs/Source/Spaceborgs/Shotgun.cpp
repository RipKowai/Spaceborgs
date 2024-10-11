// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "SpaceborgsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"

AShotgun::AShotgun()
{
	Damage = 25.0f;
	Range = 1000.0f;
	MaxAmmo = 10;
	AmmoCount = 10;
	ReloadTime = 2.0f;
	FireRate = 1.f;
	Timer = FireRate;
	IsDelaying = false;
	MuzzleFlash = nullptr;
	FireSound = nullptr;
	ReloadSound = nullptr;

	RecoilValue = 1000.f;

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


void AShotgun::Shoot(FHitResult OutHit, class ASpaceborgsCharacter* controller, UAnimMontage* ShootMontage)
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

				
				ParentActor = GetAttachParentActor();
				ASpaceborgsCharacter* ParentPlayer = Cast<ASpaceborgsCharacter>(ParentActor);

				if (ParentPlayer)
				{
					FVector ForwardVector = GetActorForwardVector();

					FVector RecoilVector = -ForwardVector * RecoilValue;

					ParentPlayer->LaunchCharacter(RecoilVector, true, true);
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
