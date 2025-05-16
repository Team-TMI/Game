// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/PlayerState.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "JumpGame/Utils/FastLogger.h"

ALobbyGameMode::ALobbyGameMode()
{
	// GamaeState, PlayerState 설정하기
	DefaultPawnClass = nullptr;
	GameStateClass = ALobbyGameState::StaticClass();
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 게임 인스턴스 가져오기
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	
	// 플레이어의 고유한 네트워크 ID값 가져오기
	const FUniqueNetIdRepl& NetIdRepl = NewPlayer->GetPlayerState<APlayerState>()->GetUniqueId();
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		// 문자열로 추출하자
		Key = NetId->ToString();

		if (IOnlineSubsystem* Subsys = IOnlineSubsystem::Get())
		{
			IOnlineIdentityPtr Identity = Subsys->GetIdentityInterface();
			if (Identity.IsValid())
			{
				FString SteamName = Identity->GetPlayerNickname(*NetId);

				// 이미 등록되었는지 확인
				if (!GI->GetPlayerInfo().Contains(SteamName))
				{
					// 플레이어 저장
					FPlayerInfo NewPlayerInfo;
					NewPlayerInfo.PlayerID = PlayerIdx;
					NewPlayerInfo.PlayerName = SteamName; // 닉네임으로 설정

					// GI에 업데이트 (서버에 저장)
					GI->AddPlayerInfo(SteamName, NewPlayerInfo);
					PlayerIdx++;
				}
			}
		}
		else
		{
			if (GI->GetPlayerInfo().Contains(Key)) return;
			
			// 플레이어 저장
			FPlayerInfo NewPlayerInfo;
			NewPlayerInfo.PlayerID = PlayerIdx;
			NewPlayerInfo.PlayerName = Key;

			// GI에 업데이트 (서버에 저장)
			GI->AddPlayerInfo(Key, NewPlayerInfo);
			PlayerIdx++;
		}
	}
}
