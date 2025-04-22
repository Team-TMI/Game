// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimitiveProp/PrimitiveProp.h"
#include "BaseProp.generated.h"

class UBoxComponent;

UCLASS()
class JUMPGAME_API ABaseProp : public APrimitiveProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetCollision(bool bEnable);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 물체의 외관 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionComp;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite)
	USceneComponent* PivotScene;
};
