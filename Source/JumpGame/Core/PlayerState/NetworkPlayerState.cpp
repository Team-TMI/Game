// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerState.h"

#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Utils/FastLogger.h"

void ANetworkPlayerState::OnSetUniqueId()
{
	Super::OnSetUniqueId();

	// 로컬 플레이어 컨트롤러를 가져온다.
	// 검사
	bool bIsLocalPlayer = false;
	
	bIsLocalPlayer |= (GetLocalRole() == ROLE_Authority && GetRemoteRole() == ROLE_SimulatedProxy);
	bIsLocalPlayer |= (GetLocalRole() == ROLE_AutonomousProxy && GetRemoteRole() == ROLE_Authority);
	if (!bIsLocalPlayer)
	{
		FFastLogger::LogConsole(TEXT("Player %s is not local player"), *GetName());
		return;
	}
		
	// 서버에 연결되었을 때 서버에 자신의 NetID를 전송한다.
	const FUniqueNetIdRepl& PlayerNetID = GetUniqueId();
	FUniqueNetIdPtr PlayerNetIDPtr = PlayerNetID.GetUniqueNetId();
	if (PlayerNetIDPtr.IsValid())
	{
		FString NetID = PlayerNetIDPtr->ToString();
		ServerRPC_HandShake(NetID);
	}
	else
	{
		FFastLogger::LogConsole(TEXT("Player %s NetID is not valid"), *GetName());
	}
}

void ANetworkPlayerState::ServerRPC_HandShake_Implementation(const FString& NetID)
{
	ANetworkGameState* GameState = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->HandleConnection(NetID);
	}
}

void ANetworkPlayerState::ServerRPC_ConnectionVerified_Implementation(const FString& NetID)
{
	ANetworkGameState* GameState = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->VerifyConnection(NetID);
	}
}
