// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolComponent.h"
#include "ObstacleProp.h"
#include "RollingCanonProp.generated.h"

UCLASS()
class JUMPGAME_API ARollingCanonProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARollingCanonProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	class UObjectPoolComponent* ObjectPool;

	// 발사하자
	void FireRollingBall();
};
