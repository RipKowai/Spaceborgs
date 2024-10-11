// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

// Sets default values
ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Damage = 50.f;
	HitRate = 0.5f;
	HitCombo = 0.f;

	Attacks.Add(TEXT("Combo1"));
	Attacks.Add(TEXT("Combo2"));
	Attacks.Add(TEXT("Combo3"));

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

	IsDelaying = false;
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDelaying)
	{
		Timer += DeltaTime;
		if (Timer > HitRate)
		{
			IsDelaying = false;
		}
	}
}

void ASword::Attack()
{
	if (Timer >= HitRate)
	{
		HitCombo++;

		if (Attacks.IsValidIndex(HitCombo))
		{
			ComboAttacks(Attacks[HitCombo]);
		}
	}

	if (HitCombo >= 3)
	{
		HitCombo = 0;
	}
}

void ASword::ComboAttacks(const FString& AttackName)
{
	if (AttackName == TEXT("Combo1"))
	{
		PlayAnimation(AttackAnimation[0]);
		UE_LOG(LogTemp, Warning, TEXT("Performing Attack 1"));
	}
	else if (AttackName == TEXT("Combo2"))
	{
		PlayAnimation(AttackAnimation[1]);
		UE_LOG(LogTemp, Warning, TEXT("Performing Attack 2"));
	}
	else if (AttackName == TEXT("Combo3"))
	{
		PlayAnimation(AttackAnimation[2]);
		UE_LOG(LogTemp, Warning, TEXT("Performing Attack 3"));
	}
	else
	{
		PlayAnimation(AttackAnimation[0]);
		UE_LOG(LogTemp, Warning, TEXT("Unknown Attack"));
	}
}

void ASword::PlayAnimation(UAnimSequence* Animation)
{
	if (Animation)
	{
		SkeletalMeshComponent->PlayAnimation(Animation, false);
	}
}

