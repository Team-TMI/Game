// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateGizmoComponent.h"

URotateGizmoComponent::URotateGizmoComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/SM_RotateGizmo.SM_RotateGizmo"));
	if (SM_GIZMO.Succeeded())
	{
		GizmoMesh = SM_GIZMO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_RotateGizmo.MI_RotateGizmo"));
	if (MI_GIZMO.Succeeded())
	{
		GizmoMaterial = MI_GIZMO.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_SELECTED
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_SelectedRotateGizmo.MI_SelectedRotateGizmo"));
	if (MI_GIZMO_SELECTED.Succeeded())
	{
		GizmoSelectedMaterial = MI_GIZMO_SELECTED.Object;
	}
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
}
