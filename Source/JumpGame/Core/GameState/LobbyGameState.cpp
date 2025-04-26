// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/UI/WaitRoomUI.h"

ALobbyGameState::ALobbyGameState()
{
	ConstructorHelpers::FClassFinder<UWaitRoomUI> TempWaitUI(TEXT("/Game/UI/LobbyUI/WBP_WaitRoomUI.WBP_WaitRoomUI_C"));
	if (TempWaitUI.Succeeded())
	{
		WaitRoomUIClass = TempWaitUI.Class;
	}
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	// GI
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	
	// 위젯 생성
	if (WaitRoomUIClass)
	{
		WaitRoomUI = CreateWidget<UWaitRoomUI>(GetWorld(), WaitRoomUIClass);
		if (WaitRoomUI)
		{
			WaitRoomUI->AddToViewport();
		}
	}
}

void ALobbyGameState::MulticastRPC_UpdateWaitImage_Implementation(const FString& PlayerKey,
	const FPlayerInfo& PlayerInfo)
{
	if (GI && !HasAuthority())
	{
		GI->AddPlayerInfo(PlayerKey, PlayerInfo);
	}
	
	if (WaitRoomUI)
	{
		WaitRoomUI->UpdateWaitPlayer();
	}
}

