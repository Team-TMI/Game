// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditorState.h"

#include "Blueprint/UserWidget.h"
#include "JumpGame/AIServices/Shared/WebSocketManageComponent.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/UI/MapEditing/MapEditingHUD.h"
#include "JumpGame/Utils/FastLogger.h"

AMapEditorState::AMapEditorState()
{
	CategorySystem = CreateDefaultSubobject<UCategorySystem>(TEXT("CategorySystem"));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_MAPEDITING_HUD
	(TEXT("/Game/UI/MapEditing/WBP_MapEditingHUD.WBP_MapEditingHUD_C"));
	if (WBP_MAPEDITING_HUD.Succeeded())
	{
		MapEditingHUDClass = WBP_MAPEDITING_HUD.Class;
	}
}

void AMapEditorState::BeginPlay()
{
	Super::BeginPlay();
	
	MapEditingHUD = CreateWidget<UMapEditingHUD>(GetWorld(), MapEditingHUDClass);
	if (MapEditingHUD)
	{
		FFastLogger::LogScreen(FColor:: Red, TEXT("MapEditingHUD Created"));
		MapEditingHUD->AddToViewport();
	}
}

void AMapEditorState::InitWidget(class UClickHandlerManager* ClickHandlerManager,
	class UWidgetMapEditDragDropOperation* DragDropOperation)
{
	if (MapEditingHUD)
	{
		MapEditingHUD->InitWidget(ClickHandlerManager, DragDropOperation);
	}
}


