﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecorationProp.h"
#include "BrightFrog.generated.h"

UCLASS()
class JUMPGAME_API ABrightFrog : public ADecorationProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABrightFrog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetCollision(bool bEnable) override;
	virtual void MaterialChangeOnTick() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	class UMaterialInstanceDynamic* UnSelectedObjectMaterial2;
	UPROPERTY()
	class UMaterialInstanceDynamic* UnSelectedObjectMaterial3;
};
