// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
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

	UPROPERTY()
	class UWebSocketManageComponent* WebSocketManageComponent = nullptr;
};
