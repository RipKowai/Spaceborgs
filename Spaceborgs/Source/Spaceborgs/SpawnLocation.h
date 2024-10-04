// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnLocation.generated.h"

UCLASS()
class SPACEBORGS_API ASpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnLocation();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemies();

	void SpawnEnemy();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> EnemyToSpawn;

	UPROPERTY(EditAnywhere)
	FVector SpawnPosition;

	UPROPERTY(EditAnywhere)
	int NumOfEnemies;

	UPROPERTY(EditAnywhere)
	float DelayTime;
};
