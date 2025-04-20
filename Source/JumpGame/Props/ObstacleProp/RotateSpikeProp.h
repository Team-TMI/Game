// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleProp.h"
#include "RotateSpikeProp.generated.h"

// 회전 + 가시가 달려있는데 가시에 부딪히면 약하게 넉백

UCLASS()
class JUMPGAME_API ARotateSpikeProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARotateSpikeProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CalculateForce(AFrog* Character) override;

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Hammer;
	// 가시
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Spike;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwingHammer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReverseSwingHammer();
};
