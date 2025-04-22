// Fill out your copyright notice in the Description page of Project Settings.


#include "GizmoClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


FGizmoClickHandler::FGizmoClickHandler()
{
}


FGizmoClickHandler::~FGizmoClickHandler()
{
}

bool FGizmoClickHandler::HandleClick(FClickResponse& ClickResponse, AMapEditingPlayerController* PlayerController)
{
	ClickResponse.Result = EClickHandlingResult::GizmoEditing;
	
	// true가 될 경우 해당 액터의 기즈모가 클릭이 된거임!
	if (PlayerController->OnClickOperation(ClickResponse.TargetProp, ClickResponse))
	{
		ClickResponse.DebugMessage = TEXT("Gizmo Click");

		FHitResult HitResult = ClickResponse.HitResult;

		ClickResponse.TargetProp = Cast<APrimitiveProp>(HitResult.GetActor());
		// 기존의 Gizmo가 선택되어 있다면 UnSelected 처리
		if (UGizmoComponent* ControlledGizmo = ClickResponse.TargetGizmo)
		{
			ControlledGizmo->SetUnSelected();
			ClickResponse.TargetGizmo = nullptr;
		}
		
		ClickResponse.TargetGizmo = Cast<UGizmoComponent>(HitResult.GetComponent());
		// 새로운 Gizmo가 선택되었다면 Selected 처리
		if (UGizmoComponent* Gizmo = ClickResponse.TargetGizmo)
		{
			Gizmo->SetSelected();
		}
		
		return true;
	}
	return false;
}
