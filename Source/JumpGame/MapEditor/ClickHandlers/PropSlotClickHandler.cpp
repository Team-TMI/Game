#include "PropSlotClickHandler.h"

#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


FPropSlotClickHandler::FPropSlotClickHandler()
{
}

FPropSlotClickHandler::~FPropSlotClickHandler()
{
}

bool FPropSlotClickHandler::HandleClick(FClickResponse& ClickResponse,
	class AMapEditingPlayerController* PlayerController)
{
	if (ClickResponse.Result != EClickHandlingResult::UIEditing)
	{
		return false;
	}
	if (ClickResponse.ClickedPropByWidget == nullptr)
	{
		return false;
	}

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

	ClickResponse.TargetProp = ClickResponse.ClickedPropByWidget;
	ClickResponse.TargetProp->SetSelected();
	
	return true;
}
