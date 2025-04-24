// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RollingBallProp.generated.h"

UCLASS()
class JUMPGAME_API ARollingBallProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARollingBallProp();

protected:
	UFUNCTION()
	void OnMyRollingBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                        const FHitResult& Hit);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 오브젝트 풀링으로 관리될 오브젝트
	// 오브젝트 풀의 레퍼런스 초기화를 위한 함수 (어디소속)
	FORCEINLINE void SetObjectPool(class UObjectPoolComponent* InObjectPool) { ObjectPool = InObjectPool; }
	void ReturnSelf();
	void SetActive(bool bIsActive);
	FORCEINLINE bool IsActive() { return bActive; }
	
private:
	// 현재 사용중인가요?
	bool bActive;
	// 소속 풀
	UPROPERTY()
	class UObjectPoolComponent* ObjectPool;

public:
	// 외관
	UPROPERTY(visibleAnywhere, BlueprintReadWrite)
	USceneComponent* PivotScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	
	// 프로젝타일로 날아가자
	UPROPERTY(visibleAnywhere,BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileComp;

	FTimerHandle PoolTimerHandle;
	void LaunchProjectile();

	// 구르자
	UPROPERTY(visibleAnywhere,BlueprintReadWrite)
	class UArrowComponent* Arrow;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	bool bIsHitGround = false;
	UPROPERTY()
	FVector LaunchDir;
	UPROPERTY()
	FVector GravityDir = FVector(0.0f, 0.0f, -1.0f);
	UPROPERTY()
	FVector GroundDir = FVector::ZeroVector;
	UPROPERTY()
	FVector HitNormal = FVector::ZeroVector;
	// 구르는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollingSpeed = 500.f;
	
	void RollingBall();
};


