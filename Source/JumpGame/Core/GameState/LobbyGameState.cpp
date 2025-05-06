// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"
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
	
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// 최대 몇명의 플레이어가 플레이를 할지 설정하는 함수 (GI에서 정보를 들고와서 설정해준다)
	// int32 MaxPlayer = GI->GetMaxPlayerCount();
	// 근데 로비니까 8명으로 강제설정
	ConnectionVerifyComponent->InitMaxPlayerCount(8);
	
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

void ALobbyGameState::OnClientAdded(const FString& NetID)
{
	Super::OnClientAdded(NetID);

	// 업데이트 내용 멀티캐스트 (모든 사람이 알 수 있게)
	TMap<FString, FPlayerInfo>& InfoMap = GI->GetPlayerInfo();
	for (auto& it : InfoMap)
	{
		MulticastRPC_UpdateWaitImage(it.Key, it.Value);
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

