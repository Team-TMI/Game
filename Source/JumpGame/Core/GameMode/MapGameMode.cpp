// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/Props/LogicProp/GameStartProp.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"

AMapGameMode::AMapGameMode()
{
	LoadMapComponent = CreateDefaultSubobject<ULoadMapComponent>(TEXT("LoadMapComponent"));
	SaveMapComponent = CreateDefaultSubobject<USaveMapComponent>(TEXT("SaveMapComponent"));

	GameStateClass = AMapGameState::StaticClass();
}

void AMapGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMapGameMode::PostLogin(APlayerController* NewPlayer)
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
				if (!GI->GetPlayerInfo().Contains(Key))
				{
					// 플레이어 저장
					FPlayerInfo NewPlayerInfo;
					NewPlayerInfo.PlayerID = PlayerIdx;
					NewPlayerInfo.PlayerName = SteamName; // 닉네임으로 설정

					// GI에 업데이트 (서버에 저장)
					GI->AddPlayerInfo(Key, NewPlayerInfo);
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
	
	/*if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		// 문자열로 추출하자
		Key = NetId->ToString();
	}

	// 이미 등록된 플레이어인지 확인
	if (GI->GetPlayerInfo().Contains(Key))
	{
		return;
	}

	// 플레이어 저장
	FPlayerInfo NewPlayerInfo;
	NewPlayerInfo.PlayerID = PlayerIdx;
	NewPlayerInfo.PlayerName = Key;
	
	// GI에 업데이트 (서버에 저장)
	GI->AddPlayerInfo(Key, NewPlayerInfo);

	PlayerIdx++;*/
}

AActor* AMapGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// 태그로 찾기
	TArray<AActor*> FoundProps;
	UGameplayStatics::GetAllActorsWithTag(this, FName("GameStart"), FoundProps);
	AGameStartProp* Prop = Cast<AGameStartProp>(FoundProps[0]);
	
	APlayerStart* TempStart = GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), Prop->PlayerStartTransform());
	return TempStart;
}

