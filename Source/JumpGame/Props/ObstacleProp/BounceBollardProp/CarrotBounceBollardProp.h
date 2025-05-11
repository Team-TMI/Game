// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Props/ObstacleProp/BounceBollardProp.h"
#include "CarrotBounceBollardProp.generated.h"

UCLASS()
class JUMPGAME_API ACarrotBounceBollardProp : public ABounceBollardProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarrotBounceBollardProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
