// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "MapEditorState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AMapEditorState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	AMapEditorState();

	virtual void BeginPlay() override;

	UFUNCTION()
	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* DragDropOperation);
private:
	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MapEditingHUDClass = nullptr;
	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMapEditingHUD* MapEditingHUD = nullptr;
};
