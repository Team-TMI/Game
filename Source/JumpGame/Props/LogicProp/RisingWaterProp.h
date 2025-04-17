// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "RisingWaterProp.generated.h"

UENUM()
enum class EWaterStateEnum : uint8
{
	None,
	Rise
};

/**
 * 
 */
UCLASS()
class JUMPGAME_API ARisingWaterProp : public ALogicProp
{
	GENERATED_BODY()

public:
	ARisingWaterProp();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

	UFUNCTION()
	void OnBeginDeepOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp,
	                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndDeepOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex);
	UFUNCTION()
	void OnBeginShallowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndShallowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	void OnBeginSurfaceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndSurfaceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION()
	void InDeepWater(float DeltaTime);
	UFUNCTION()
	void InShallowWater(float DeltaTime);
	UFUNCTION()
	void InSurfaceWater(float DeltaTime);

	UFUNCTION()
	void RiseWater(float DeltaTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* DeepCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* ShallowCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* SurfaceCollision;

public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// ECharacterStateEnum CharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWaterStateEnum WaterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AFrog* Frog;
};
