// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnLocation.h"

#include "Engine/World.h"

// Sets default values
ASpawnLocation::ASpawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawnLocation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnLocation::SpawnEnemies()
{
	for (int i = 0; i < NumOfEnemies; ++i)
	{
		//FTimerHandle TimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpawnLocation::SpawnEnemy, DelayTime * i, false);
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(EnemyToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}

//void ASpawnLocation::SpawnEnemy()
//{
//	FActorSpawnParameters SpawnParams;
//	GetWorld()->SpawnActor<AActor>(EnemyToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
//}

