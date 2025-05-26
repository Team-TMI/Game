#include "RotateGizmoClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/RotateGizmoComponent.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/MapEditor/RotateHandlers/RotateHandlerManager.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

FRotateGizmoClickHandler::FRotateGizmoClickHandler()
{
}

bool FRotateGizmoClickHandler::HandleClick(FClickResponse& ClickResponse,
	class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode)
{
	ClickResponse.Result = EClickHandlingResult::RotateGizmoEditing;
	
	FFastLogger::LogScreen(FColor::Red, TEXT("Rotate Gizmo Clicked"));
	if (!bRotateGizmoMode)
	{
		return false;
	}

	if (PlayerController->OnClickOperation(ClickResponse.TargetProp, ClickResponse))
	{
		FHitResult HitResult = ClickResponse.HitResult;

		FFastLogger::LogScreen(FColor::Red, TEXT("Rotate Gizmo Clicked1"));
		ClickResponse.TargetProp = Cast<APrimitiveProp>(HitResult.GetActor());

		// UGizmoComponent* RotateGizmo = ClickResponse.TargetGizmo;
		// if (!RotateGizmo)
		// {
		// 	return false;
		// }
		URotateGizmoComponent* RotateGizmoComponent = Cast<URotateGizmoComponent>(HitResult.GetComponent());
		if (!RotateGizmoComponent)
		{
			return false;
		}
		FFastLogger::LogScreen(FColor::Red, TEXT("Rotate Gizmo Clicked2"));

		RotateGizmoComponent->SetSelected();

		// Rotate 할 축을 들고옴
		// FVector Direction = RotateGizmo->GetDirection();

		AMapEditingPawn* Editor = Cast<AMapEditingPawn>(PlayerController->GetPawn());
		if (!Editor)
		{
			return false;
		}
		
		FFastLogger::LogScreen(FColor::Red, TEXT("Rotate Gizmo Clicked3"));
		Editor->GetRotateHandlerManager()->HandleRotate(ClickResponse);
		
		return true;
	}
	
	FFastLogger::LogScreen(FColor::Red, TEXT("Rotate Gizmo Clicked4"));
	return false;
}

FRotateGizmoClickHandler::~FRotateGizmoClickHandler()
{
}
