#include "ClickHandlerManager.h"

#include "ActorClickHandler.h"
#include "BackgroundClickHandler.h"
#include "GizmoClickHandler.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/Utils/FastLogger.h"

UClickHandlerManager::UClickHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UClickHandlerManager::RegisterHandler(TSharedPtr<IClickHandler> Handler)
{
	Handlers.Add(Handler);
	Handlers.Sort([](const TSharedPtr<IClickHandler>& A, const TSharedPtr<IClickHandler>& B)
	{
		return A->GetPriority() > B->GetPriority();
	});
}

bool UClickHandlerManager::HandleClick(class AMapEditingPlayerController* PlayerController)
{
	AActor* TempActor = ControlledClickResponse.TargetProp;
	
	bool bHandled = false;
	for (const auto& Handler : Handlers)
	{
		if (Handler->HandleClick(ControlledClickResponse, PlayerController))
		{
			bHandled = true;
			break ;
		}
	}
	
	if (TempActor != ControlledClickResponse.TargetProp)
	{
		OnControlledPropChanged.Broadcast();
	}
	
	return bHandled;
}

void UClickHandlerManager::ResetControl()
{
	ControlledClickResponse = FClickResponse();
	OnControlledPropChanged.Broadcast();
}

void UClickHandlerManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterHandler(MakeShared<FGizmoClickHandler>());
	RegisterHandler(MakeShared<FActorClickHandler>());
	RegisterHandler(MakeShared<FBackgroundClickHandler>());
}
