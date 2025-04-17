// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "VictoryPlatform.generated.h"

UCLASS()
class JUMPGAME_API AVictoryPlatform : public ALogicProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVictoryPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

};
