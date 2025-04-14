// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameState.h"

#include "JumpGame/Core/PlayerController/NetworkPlayerController.h"

ANetworkGameState::ANetworkGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

double ANetworkGameState::GetServerWorldTimeSeconds() const
{
	// 기존의 시간을 가져오는 함수를 사용하지 않고, 서버 시간을 가져오도록 변경한다.
	// return Super::GetServerWorldTimeSeconds();

	// 서버 자기 자신이 요청하는 경우 서버 시간을 반환한다.
	if (GetLocalRole() == ROLE_Authority)
	{
		return ServerWorldTimeSeconds;
	}

	// 클라이언트가 요청하는 경우 클라이언트가 서버와 직접 동기화된 시간을 가져온다.
	// FirstPlayerController를 가져오는 이유는 해당 플레이어 컨트롤러에서만 관리되기 때문이다.
	if(ANetworkPlayerController* PC = Cast<ANetworkPlayerController>(GetGameInstance()->GetFirstLocalPlayerController(GetWorld())))
	{
		return PC->GetServerTime();
	}
	else
	{
		return GetWorld()->GetTimeSeconds();
	}
}
