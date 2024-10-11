// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class SPACEBORGS_API ASword : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASword();
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Attacks")
	TArray<UAnimSequence*> AttackAnimation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitRate;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	float HitCombo;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;



private:
	UPROPERTY(VisibleAnywhere)
	float Timer;

	UPROPERTY(VisibleAnywhere)
	float IsDelaying;


	TArray<FString> Attacks;

	UFUNCTION(BlueprintCallable)
	void ComboAttacks(const FString& AttackName);

	// Function to play the animation
	UFUNCTION(BlueprintCallable)
	void PlayAnimation(UAnimSequence* Animation);
};
