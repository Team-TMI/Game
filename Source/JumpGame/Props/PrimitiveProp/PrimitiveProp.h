// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "PrimitiveProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API APrimitiveProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APrimitiveProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 액터가 선택되었을 때 호출되는 함수
	// 내부 Mesh 컴포넌트의 색상 및 충돌 설정
	virtual void SetCollision(bool bEnable) {};


	UFUNCTION(BlueprintCallable)
	void SetSize(const FVector& InSize);
	UFUNCTION(BlueprintCallable)
	void SetSelected();
	UFUNCTION(BlueprintCallable)
	void SetUnSelected();
	UFUNCTION(BlueprintCallable)
	void SetPrimitivePropCollision(bool bCond);
	UFUNCTION(BlueprintCallable)
	void SetGizmosCollision(bool bCond);
	UFUNCTION(BlueprintCallable)
	void RotateAllGizmos();
	UFUNCTION(BlueprintCallable)
	void SetNewSizeByRotation(const FVector& InSize);

	GETTER(class UBoxComponent*, GridOuterCollision)
	GETTER(class UBoxComponent*, GridInnerCollision);
	GETTER(class UGridComponent*, GridComp);
	GETTER(class UPropDataComponent*, PropDataComponent);

protected:
	UFUNCTION()
	void SetGizmoLocation(class UGizmoComponent* Gizmo, const FVector& Direction, const FVector& InBoxSize);
	UFUNCTION()
	void SetGizmoRotation(class UGizmoComponent* Gizmo, const FVector& Direction);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGridComponent* GridComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoPrimary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoTwo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoThree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoFour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoFive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	class UGizmoComponent* GizmoSix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmos")
	TArray<class UGizmoComponent*> GizmoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class UBoxComponent* GridOuterCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class UBoxComponent* GridInnerCollision;

	// 1 * 1 * 1 size
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	FVector DefaultSize = FVector(1.0f, 1.0f, 1.0f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	FVector DefaultCollisionExtentOffset = FVector(1, 1, 1);

	UPROPERTY()
	TArray<FVector> GizmoDirectionArray = {
		FVector(1, 0, 0),  // +X
		FVector(-1, 0, 0), // -X
		FVector(0, 1, 0),  // +Y
		FVector(0, -1, 0), // -Y
		FVector(0, 0, 1),  // +Z
		FVector(0, 0, -1)  // -Z
	};

	UPROPERTY()
	bool bSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PropDataComponent")
	class UPropDataComponent* PropDataComponent = nullptr;
};
