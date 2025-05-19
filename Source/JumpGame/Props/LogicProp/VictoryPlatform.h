// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "VictoryPlatform.generated.h"

UCLASS(Blueprintable)
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
	// 골인지점에서 구현
	// void SpawnVictoryPlatform();

	// 카메라 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* VictoryCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VictoryPlane;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VictoryCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* VictoryArrow;
	
	// 1등 유저를 소환할 위치값을 반환하는 함수
	FVector SpawnVictoryCharacter();
};
