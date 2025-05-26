// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateGizmoComponent.h"

URotateGizmoComponent::URotateGizmoComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ROTATE_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/SM_RotateGizmo.SM_RotateGizmo"));
	if (SM_ROTATE_GIZMO.Succeeded())
	{
		GizmoMesh = SM_ROTATE_GIZMO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_ROTATE
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_RotateGizmo.MI_RotateGizmo"));
	if (MI_GIZMO_ROTATE.Succeeded())
	{
		GizmoMaterial = MI_GIZMO_ROTATE.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_SELECTED_ROTATE
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_SelectedRotateGizmo.MI_SelectedRotateGizmo"));
	if (MI_GIZMO_SELECTED_ROTATE.Succeeded())
	{
		GizmoSelectedMaterial = MI_GIZMO_SELECTED_ROTATE.Object;
	}

	Super::SetStaticMesh(GizmoMesh);
	Super::SetMaterial(0, GizmoMaterial);
	SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
}

FVector URotateGizmoComponent::GetDirection() const
{
	return AxisDirection.GetSafeNormal();
}

void URotateGizmoComponent::SetSelected()
{
	Super::SetSelected();
}

void URotateGizmoComponent::SetUnSelected()
{
	Super::SetUnSelected();

	// 축을 0으로 초기화 : Yaw 축 회전으로 초기화
	AxisDirection = FVector(0, 0, 1);
	SetRelativeRotation(FRotator(0, 0, 0));

	// Pitch : 90 Yaw : 0 Roll : 0 => Roll 축 회전으로 초기화
	// Pitch : 0 Yaw : 0 Roll : 90 => Pitch 축 회전으로 초기화
	// Pitch : 0 Yaw : 0 Roll : 0 => Yaw 축 회전으로 초기화
	// SetRelativeRotation(FRotator(0, 0, 90)); => Pitch 축 회전으로 초기화
}

void URotateGizmoComponent::SetAxisDirection(const FVector& NewAxisDirection)
{
	AxisDirection = NewAxisDirection;
	if (AxisDirection.X)
	{
		SetRelativeRotation(FRotator(-90, 0, 0));
	}
	else if (AxisDirection.Y)
	{
		SetRelativeRotation(FRotator(0, 0, -90));
	}
	else if (AxisDirection.Z)
	{
		SetRelativeRotation(FRotator(0, 0, 0));
	}
}
