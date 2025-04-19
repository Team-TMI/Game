#include "ClickHandlerManager.h"

#include "ActorClickHandler.h"
#include "BackgroundClickHandler.h"
#include "GizmoClickHandler.h"
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
	for (const auto& Handler : Handlers)
	{
		if (Handler->HandleClick(ControlledClickResponse, PlayerController))
		{
			return true;
		}
	}
	return false;
}

void UClickHandlerManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterHandler(MakeShared<FGizmoClickHandler>());
	RegisterHandler(MakeShared<FActorClickHandler>());
	RegisterHandler(MakeShared<FBackgroundClickHandler>());
}
