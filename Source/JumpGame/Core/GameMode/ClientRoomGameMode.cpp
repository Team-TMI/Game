// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomGameMode.h"

#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/ClientRoomGameState.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "JumpGame/UI/ClientRoomLogoUI.h"

void AClientRoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerClass = ALobbyPlayerController::StaticClass();

	// 게임 인스턴스 가져오기
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());

	ClientRoomLogoUI = CreateWidget<UClientRoomLogoUI>(GetWorld(), ClientRoomLogoUIClass);
	if (ClientRoomLogoUI)
	{
		if (!GI->bIsGameStart)
		{
			ClientRoomLogoUI->AddToViewport(20);
			GI->bIsGameStart = true;
		}
	}
}
