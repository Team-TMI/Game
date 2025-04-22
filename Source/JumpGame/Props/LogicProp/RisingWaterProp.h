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
	void OnBeginShallowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBeginSurfaceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBeginDeadZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SetCollision(bool bEnable) override;

public:
	UFUNCTION()
	void RiseWater(float DeltaTime);
	UFUNCTION()
	void StopRising(float Time = -1.f);
	UFUNCTION()
	void StartRising();
	UFUNCTION()
	void SetRisingSpeed(float Speed);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* DeepCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* ShallowCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* SurfaceCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* DeadZoneCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* RespawnPoint;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWaterStateEnum WaterState;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AFrog* Frog;

public:
	FTimerHandle TimerHandle;
	FTimerHandle StopTimerHandle;
	float FlowTime{};
	float JumpTime{2.f};
	bool bIsChanged{false};
	float RisingSpeed{50.f};
};
