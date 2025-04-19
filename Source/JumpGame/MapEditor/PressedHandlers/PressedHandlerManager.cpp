#include "PressedHandlerManager.h"

#include "GizmoPressedHandler.h"
#include "GizmoPrimaryPressedHandler.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


UPressedHandlerManager::UPressedHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPressedHandlerManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterHandler(MakeShared<FGizmoPrimaryPressedHandler>());
	RegisterHandler(MakeShared<FGizmoPressedHandler>());
}

void UPressedHandlerManager::RegisterHandler(TSharedPtr<IPressedHandler> Handler)
{
	Handlers.Add(Handler);
	Handlers.Sort([](const TSharedPtr<IPressedHandler>& A, const TSharedPtr<IPressedHandler>& B)
	{
		return A->GetPriority() > B->GetPriority();
	});
}

bool UPressedHandlerManager::HandlePressed(FClickResponse& ControlledInfo,
	class AMapEditingPlayerController* PlayerController)
{
	for (const auto& Handler : Handlers)
	{
		if (Handler->HandlePressed(ControlledInfo, PlayerController, MouseStartPosition, InitializeActorPosition))
		{
			return true;
		}
	}
	return false;
}

void UPressedHandlerManager::InitializePositions(FClickResponse& ControlledInfo, AMapEditingPlayerController* PlayerController)
{
	PlayerController->GetWorldMousePosition(MouseStartPosition);

	if (ControlledInfo.TargetProp)
	{
		InitializeActorPosition = ControlledInfo.TargetProp->GetActorLocation();
	}
}

void UPressedHandlerManager::ResetPositions()
{
	MouseStartPosition = FVector::ZeroVector;
	InitializeActorPosition = FVector::ZeroVector;
}
