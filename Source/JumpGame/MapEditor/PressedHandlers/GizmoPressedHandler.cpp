// Fill out your copyright notice in the Description page of Project Settings.


#include "GizmoPressedHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/ClickHandlers/GizmoClickHandler.h"
#include "JumpGame/MapEditor/Components/GizmoPrimaryComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


FGizmoPressedHandler::FGizmoPressedHandler()
{
}

FGizmoPressedHandler::~FGizmoPressedHandler()
{
}

bool FGizmoPressedHandler::HandlePressed(FClickResponse& PressedResponse,
	class AMapEditingPlayerController* PlayerController, const FVector& MouseStartPosition, const FVector& InitializedActorPosition)
{
	if (PressedResponse.Result != EClickHandlingResult::GizmoEditing)
	{
		return false;
	}
	// 그럴 일은 없지만 애초에 이게 들어오면 전체 로직이 잘못된거임!
	if (PressedResponse.TargetGizmo->IsA(UGizmoPrimaryComponent::StaticClass()))
	{
		return false;
	}

	UGizmoComponent* Gizmo = PressedResponse.TargetGizmo;
	if (!Gizmo)
	{
		return false;
	}

	// PressedResponse.TargetProp->SetGizmosCollision(false);
	// PressedResponse.TargetProp->SetGizmosCollision(true);

	FVector Direction = Gizmo->GetDirection().GetAbs();

	FHitResult HitResult;
	if (!PlayerController->OnPressedOperation(PressedType, HitResult))
	{
		return false;
	}

	FVector CurrentMouseLocation = HitResult.Location;
	FVector MouseDelta = CurrentMouseLocation - MouseStartPosition;

	// 마우스가 방향으로 얼마나 이동했는지 거리 측정
	float Distance = FVector::DotProduct(MouseDelta, Direction.GetSafeNormal());

	// 100 이상 움직였을 때만 1단위 스냅
	int32 Snap = FMath::RoundToInt(Distance / 100.0f); // 또는 Trunc/Sign, 상황에 맞게

	// 최종 이동 좌표 (X, Y, Z 중 하나만 ±1, 나머지는 0)
	FVector StepVector = Direction.GetSafeNormal() * (float)Snap;

	// 정수로 바꿔서 이동 처리 (각 축 중 하나만 의미 있음)
	FVector StepVectorInt = FVector(
		FMath::RoundToInt(StepVector.X),
		FMath::RoundToInt(StepVector.Y),
		FMath::RoundToInt(StepVector.Z));

	TWeakObjectPtr<UGridComponent> GridComponent = PressedResponse.TargetProp->GetGridComp();
	UGridComponent* Grid = GridComponent.Get();
	if (!Grid)
	{
		return false;
	}

	Grid->MoveByGizmo(StepVectorInt);
	
	return true;
}
