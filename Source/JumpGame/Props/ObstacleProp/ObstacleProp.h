// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Props/BaseProp.h"
#include "ObstacleProp.generated.h"

class AFrog;

UCLASS()
class JUMPGAME_API AObstacleProp : public ABaseProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstacleProp();

protected:
	UFUNCTION()
	void OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Launch 관련
	// 장애물: 트램펄린, 튕기기막대, 회전망치
	// LaunchVelocity를 초기값으로 주면 튕겨내지 않음
	UPROPERTY(EditAnywhere, Category = "Launch")
	FVector LaunchVelocity	= FVector(0, 0, 0);
	
	// 디버깅
	UPROPERTY(EditAnywhere)
	bool bDebug = false;
	
	// Rotate (자체 회전)관련
	// 장애물: 회전망치, 뿔망치, 굴러오는 공
	UPROPERTY(EditAnywhere, Category = "Rotate")
	float RotAngle = 0;
	UPROPERTY (EditAnywhere, Category = "Rotate")
	FRotator RotAxis = FRotator(0, 0, 0);
	
	virtual void LaunchCharacter(AFrog* Character, FVector Direction, float Force, bool XYOverride = false, bool ZOverride = false);
	virtual void CalculateForce(AFrog* Character);
	virtual void ObstacleRotate();
};
