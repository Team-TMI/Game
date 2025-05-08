// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMode.h"

#include "JumpGame/Core/PlayerController/InGamePlayerController.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"

AInGameMode::AInGameMode()
{
	PlayerControllerClass = AInGamePlayerController::StaticClass();
}

void AInGameMode::BeginPlay()
{
	Super::BeginPlay();

	// LoadMapComponent->LoadMap();

	// FString FileName =;
	// LoadMapComponent->LoadMapWithString(FileName);
}
