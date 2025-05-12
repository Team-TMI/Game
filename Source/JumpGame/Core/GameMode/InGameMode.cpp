// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMode.h"

#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/PlayerController/InGamePlayerController.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"

AInGameMode::AInGameMode()
{
	PlayerControllerClass = AInGamePlayerController::StaticClass();
}

void AInGameMode::BeginPlay()
{
	Super::BeginPlay();

	// LoadMapComponent->LoadMap();

	FString GameName = TEXT("ProtoGameMap");
	SaveMapComponent->SaveMap(GameName);

	UJumpGameInstance* JumpGameInstance = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	if (!JumpGameInstance)
	{
		return;
	}
	FString FileName = JumpGameInstance->GetMapFilePath();
	if (FileName.IsEmpty())
	{
		return;
	}
	LoadMapComponent->LoadMapWithString(FileName);
}
