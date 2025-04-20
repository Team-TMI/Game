// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolComponent.h"
#include "RollingCannonProp.generated.h"

UCLASS()
class JUMPGAME_API ARollingCannonProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARollingCannonProp();

protected:
	UFUNCTION()
	void OnProjectileReturn();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 외관 설정
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;
	
	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	class UObjectPoolComponent* ObjectPool;

	// 발사하자
	void FireRollingBall();
};
