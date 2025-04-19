// Fill out your copyright notice in the Description page of Project Settings.


#include "BackgroundClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


FBackgroundClickHandler::FBackgroundClickHandler()
{
}

FBackgroundClickHandler::~FBackgroundClickHandler()
{
}

bool FBackgroundClickHandler::HandleClick(FClickResponse& ClickResponse,
	class AMapEditingPlayerController* PlayerController)
{
	ClickResponse.Result = EClickHandlingResult::BackgroundEditing;
	
	// true가 될 경우 액터가 선택됨
	if (PlayerController->OnClickOperation(ClickResponse.TargetProp, ClickResponse))
	{
		ClickResponse.DebugMessage = TEXT("Background Click");

		// 기존의 Gizmo가 선택되어 있다면 UnSelected 처리
		if (UGizmoComponent* ControlledGizmo = ClickResponse.TargetGizmo)
		{
			ControlledGizmo->SetUnSelected();
			ClickResponse.TargetGizmo = nullptr;
		}

		// 기존의 액터가 선택되어 있다면 UnSelected 처리
		if (APrimitiveProp* ControlledActor = ClickResponse.TargetProp)
		{
			ControlledActor->SetUnSelected();
			ClickResponse.TargetProp = nullptr;
		}
		
		return true;
	}
	return false;
}