// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleProp.h"
#include "BounceBollard.generated.h"

UCLASS()
class JUMPGAME_API ABounceBollard : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABounceBollard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void CalculateForce(AFrog* Character) override;
};
