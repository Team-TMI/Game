// Fill out your copyright notice in the Description page of Project Settings.


#include "GizmoComponent.h"

#include "JumpGame/Utils/FastLogger.h"

UGizmoComponent::UGizmoComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/SM_Gizmo2.SM_Gizmo2"));
	if (SM_GIZMO.Succeeded())
	{
		GizmoMesh = SM_GIZMO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_Gizmo.MI_Gizmo"));
	if (MI_GIZMO.Succeeded())
	{
		GizmoMaterial = MI_GIZMO.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_GIZMO_SELECTED
	(TEXT("/Game/Fab/Cube_World_Powerup_Coin_-_Proto_Series/MI_SelectedGizmo.MI_SelectedGizmo"));
	if (MI_GIZMO_SELECTED.Succeeded())
	{
		GizmoSelectedMaterial = MI_GIZMO_SELECTED.Object;
	}
	
	Super::SetStaticMesh(GizmoMesh);
	Super::SetCollisionProfileName(TEXT("GizmoPrefet"));
	Super::SetMaterial(0, GizmoMaterial);
	SetRelativeScale3D({0.5f, 0.25f, 0.5f});
}

FVector UGizmoComponent::GetDirection() const
{
	FVector Direction = GetUpVector();
	Direction.Normalize();
	return Direction;
}

void UGizmoComponent::SetSelected()
{
	bSelected = true;

	Super::SetMaterial(0, GizmoSelectedMaterial);
}

void UGizmoComponent::SetUnSelected()
{
	bSelected = false;
	
	Super::SetMaterial(0, GizmoMaterial);
}
