// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Utils/FastLogger.h"

ALobbyGameMode::ALobbyGameMode()
{
	// GamaeState, PlayerState 설정하기
	GameStateClass = ALobbyGameState::StaticClass();
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
	}

	// 플레이어 저장
	FPlayerInfo NewPlayerInfo;
	NewPlayerInfo.PlayerID = PlayerIdx;
	NewPlayerInfo.PlayerName = Key;
	
	// GI에 업데이트 (서버에 저장)
	GI->AddPlayerInfo(Key, NewPlayerInfo);

	// 업데이트 내용 멀티캐스트 (모든 사람이 알 수 있게)
	ALobbyGameState* GS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		FTimerHandle TimerHandle;
		TWeakObjectPtr<ALobbyGameState> WeakGameState = GS;
		TWeakObjectPtr<ALobbyGameMode> WeakThis = this;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([WeakGameState, WeakThis, &NewPlayerInfo]()
		{
			TMap<FString, FPlayerInfo>& InfoMap = WeakThis->GI->GetPlayerInfo();
			for (auto& it : InfoMap)
			{
				WeakGameState->MulticastRPC_UpdateWaitImage(it.Key, it.Value);
			}
		}),1.0f, false);
	}
	else
	{
		FFastLogger::LogConsole(TEXT("GameState is NULL"));
	}

	PlayerIdx++;
}
