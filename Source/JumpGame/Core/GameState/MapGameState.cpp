// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameState.h"

#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"
#include "JumpGame/UI/GameProgressBarUI.h"

void AMapGameState::BeginPlay()
{
	Super::BeginPlay();
	
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// 최대 몇명의 플레이어가 플레이를 할지 설정하는 함수 (GI에서 정보를 들고와서 설정해준다)
	int32 MaxPlayer = GI->GetMaxPlayerCount();
	ConnectionVerifyComponent->InitMaxPlayerCount(MaxPlayer);
}

// 서버에서 호출되는 함수
void AMapGameState::OnAllClientAdded()
{
	Super::OnAllClientAdded();

	// 클라이언트에게 알리자 (2초후)
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMapGameState::MulticastRPC_AllClientAdded, 2, false);
}

void AMapGameState::MulticastRPC_AllClientAdded_Implementation()
{
	// 브로드캐스트 하자
	OnAllClientAddedDelegate.Broadcast();
}
