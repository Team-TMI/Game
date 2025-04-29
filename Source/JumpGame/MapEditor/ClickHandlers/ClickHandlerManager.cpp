#include "ClickHandlerManager.h"

#include "ActorClickHandler.h"
#include "BackgroundClickHandler.h"
#include "GizmoClickHandler.h"
#include "PropSlotClickHandler.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/Utils/FastLogger.h"

UClickHandlerManager::UClickHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UClickHandlerManager::RegisterHandler(TSharedPtr<IClickHandler> Handler)
{
	Handlers.Add(Handler);
	Handlers.Sort([](const TSharedPtr<IClickHandler>& A, const TSharedPtr<IClickHandler>& B)
	{
		return A->GetPriority() > B->GetPriority();
	});
}

bool UClickHandlerManager::HandleClick(AMapEditingPlayerController* PlayerController)
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

	RegisterHandler(MakeShared<FPropSlotClickHandler>());
	RegisterHandler(MakeShared<FGizmoClickHandler>());
	RegisterHandler(MakeShared<FActorClickHandler>());
	RegisterHandler(MakeShared<FBackgroundClickHandler>());
}

void UClickHandlerManager::InitializeComponent()
{
	Super::InitializeComponent();

	AMapEditingPawn* MapEditingPawn = Cast<AMapEditingPawn>(GetOwner());
	MapEditingPawn->GetDragDropOperation()->OnDragCancelledWidget.AddDynamic(this, &UClickHandlerManager::OnPropDragCancelled);
}


void UClickHandlerManager::OnPropSlotClicked(FName PropID, UClass* InPropClass)
{
	ControlledPropSlotID = PropID;
	ControlledPropClass = InPropClass;
}

void UClickHandlerManager::OnWidgetDragLeave()
{
	bMouseEnterUI = false;
	
	ControlledClickResponse.TargetProp->SetActorHiddenInGame(false);
}

void UClickHandlerManager::OnWidgetDragEnter()
{
	bMouseEnterUI = true;

	if (ControlledClickResponse.Result != EClickHandlingResult::UIEditing)
	{
		ControlledClickResponse.Result = EClickHandlingResult::UIEditing;
		
		APrimitiveProp* Prop = GetWorld()->SpawnActor<APrimitiveProp>(ControlledPropClass, FVector::ZeroVector, FRotator::ZeroRotator);
		ControlledClickResponse.ClickedPropByWidget = Prop;
		
		AMapEditingPlayerController* PlayerController = Cast<AMapEditingPlayerController>(GetWorld()->GetFirstPlayerController());
		HandleClick(PlayerController);
	}

	ControlledClickResponse.TargetProp->SetActorHiddenInGame(true);
}

void UClickHandlerManager::OnPropDragCancelled()
{
	ControlledPropSlotID = NAME_None;
	ControlledPropClass = nullptr;
	
	if (bMouseEnterUI)
	{
		ControlledClickResponse.TargetProp->Destroy();
		ControlledClickResponse = FClickResponse();
	}

	ControlledClickResponse.Result = EClickHandlingResult::None;
	ControlledClickResponse.ClickedPropByWidget = nullptr;
}
