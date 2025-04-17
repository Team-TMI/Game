// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameMode.h"

#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/PlayerController/NetworkPlayerController.h"

ANetworkGameMode::ANetworkGameMode()
{
	PlayerControllerClass = ANetworkPlayerController::StaticClass();
	DefaultPawnClass = AFrog::StaticClass();
}

void ANetworkGameMode::BeginPlay()
{
	Super::BeginPlay();

	// hide mouse cursor
	APlayerController* PlayerController{GetWorld()->GetFirstPlayerController()};
	if (PlayerController) {
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
	}
}
