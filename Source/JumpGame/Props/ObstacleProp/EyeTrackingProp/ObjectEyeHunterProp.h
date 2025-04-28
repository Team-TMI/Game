// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EyeTrackingProp.h"
#include "ObjectEyeHunterProp.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AObjectEyeHunterProp : public AEyeTrackingProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObjectEyeHunterProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp,
								  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual void RecvEyeTrackingInfo() override;

public:
	void FlyingObjectMovement(float DeltaTime);
	void TrackLocation(FVector2f Resolution, FVector2f ScreenLoc);
	bool IsObjectAndEyeOverlap(FVector2D ObjectLocation, FVector2D EyeLocation);
	// 수치 변화시키는 함수
	void ChangeValue(bool bIsOverlap);
	void StartMission();
	void EndMission(bool bIsSuccess);
	void StopCharacter();
	void ResumeCharacter();
	void ResetMission();
	void InitializeMission();

public:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* MissionLocation;
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEyeTrackingUI> EyeTrackingUIClass;
	UPROPERTY(EditAnywhere)
	UEyeTrackingUI* TrackingUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFlyingObjectUI> FlyingObjectUIClass;
	UPROPERTY(EditAnywhere)
	UFlyingObjectUI* FlyingObjectUI;

public:
	UPROPERTY(EditAnywhere)
	class AFrog* Frog;
	UPROPERTY(EditAnywhere)
	bool bIsDebug{false};
	FVector2D ObjectScreenLocation;
	FVector2D EyeScreenLocation;
	float SuccessRatio{0.f};
	float SuccessTime{3.f};
	float FlowTime{0.f};

	FVector2D ViewportSize;

	// Flying Object
	float BezierAlpha{0.f};
	UPROPERTY(EditAnywhere)
	float ObjectSpeed;
	FTimerHandle MovementTimerHandle;
	FVector2D StartPosition;
	FVector2D TargetPosition;
	FVector2D ControlPoint1;
	FVector2D ControlPoint2;
	TArray<FVector2D> TargetPositions;
	int32 CurrentTargetIndex;

	void SetNextTargetPosition();
	void UpdateObjectRotation(float DeltaTime);
	FVector2D GetBezierPoint(FVector2D P0, FVector2D P1, FVector2D P2, FVector2D P3, float t);
	FVector2D GenerateRandomControlPoint(FVector2D StartPos, FVector2D EndPos, float RandomRadius);
};
