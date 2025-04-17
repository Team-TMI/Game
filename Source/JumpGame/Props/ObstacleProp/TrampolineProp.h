// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleProp.h"
#include "TrampolineProp.generated.h"

UCLASS()
class JUMPGAME_API ATrampolineProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrampolineProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void CalculateForce(AFrog* Character) override;
};
