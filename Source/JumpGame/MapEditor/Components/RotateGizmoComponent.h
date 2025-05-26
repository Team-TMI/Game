// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GizmoComponent.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "RotateGizmoComponent.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API URotateGizmoComponent : public UGizmoComponent
{
	GENERATED_BODY()

public:
	URotateGizmoComponent();

	virtual FVector GetDirection() const override;
	virtual void SetSelected() override;
	virtual void SetUnSelected() override;

	void SetAxisDirection(const FVector& NewAxisDirection);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmo", meta = (AllowPrivateAccess = "true"))
	FVector AxisDirection = FVector(0, 0, 1); // Default to Z-axis
};
