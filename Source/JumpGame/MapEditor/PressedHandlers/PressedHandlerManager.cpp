#include "PressedHandlerManager.h"

#include "GizmoPrimaryPressedHandler.h"


UPressedHandlerManager::UPressedHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPressedHandlerManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterHandler(MakeShared<FGizmoPrimaryPressedHandler>());
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
		if (Handler->HandlePressed(ControlledInfo, PlayerController))
		{
			return true;
		}
	}
	return false;
}
