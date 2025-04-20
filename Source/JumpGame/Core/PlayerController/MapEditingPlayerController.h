// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkPlayerController.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "JumpGame/MapEditor/ClickHandlers/EClickHandlingResult.h"
#include "JumpGame/MapEditor/PressedHandlers/EPressedHandlingResult.h"
#include "MapEditingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AMapEditingPlayerController : public ANetworkPlayerController
{
	GENERATED_BODY()

public:
	AMapEditingPlayerController();

#pragma region CLICK_OPERATION
	// Click Operation
public:
	UFUNCTION()
	bool OnClickOperation(class APrimitiveProp* InControlledProp, FClickResponse& ClickResponse);

	UFUNCTION()
	bool OnGizmoClickOperation(class APrimitiveProp* InControlledProp, FClickResponse& ClickResponse);
	UFUNCTION()
	bool OnActorClickOperation(class APrimitiveProp* InControlledProp, FClickResponse& ClickResponse);
	UFUNCTION()
	bool OnBackgroundClickOperation(class APrimitiveProp* InControlledProp, FClickResponse& ClickResponse);

private:
	using FClickOpFunc = bool (AMapEditingPlayerController::*)(class APrimitiveProp*, FClickResponse&);
	// Click Operation
	TMap<EClickHandlingResult, FClickOpFunc> ClickOperations = {
		{ EClickHandlingResult::GizmoEditing, &AMapEditingPlayerController::OnGizmoClickOperation },
		{ EClickHandlingResult::ActorEditing, &AMapEditingPlayerController::OnActorClickOperation },
		{ EClickHandlingResult::BackgroundEditing, &AMapEditingPlayerController::OnBackgroundClickOperation }
	};
public:
#pragma endregion

#pragma region PRESSED_OPERATION
public:
	UFUNCTION()
	bool OnPressedOperation(const EPressedHandlingResult EPressedOperation, FHitResult& InHitResult);

	UFUNCTION()
	bool OnGizmoPrimaryPressedOperation(FHitResult& InHitResult);
	UFUNCTION()
	bool OnGizmoPressedOperation(FHitResult& InHitResult);

private:
	using FPressedOpFunc = bool (AMapEditingPlayerController::*)(FHitResult&);

	TMap<EPressedHandlingResult, FPressedOpFunc> PressedOperations = {
		{ EPressedHandlingResult::GizmoPrimaryPressed, &AMapEditingPlayerController::OnGizmoPrimaryPressedOperation },
		{ EPressedHandlingResult::GizmoPressed, &AMapEditingPlayerController::OnGizmoPressedOperation },
	};
#pragma endregion
	
};
