// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameState.h"

#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"

void AMapGameState::BeginPlay()
{
	Super::BeginPlay();
	
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// 최대 몇명의 플레이어가 플레이를 할지 설정하는 함수 (GI에서 정보를 들고와서 설정해준다)
	int32 MaxPlayer = GI->GetMaxPlayerCount();
	ConnectionVerifyComponent->InitMaxPlayerCount(MaxPlayer);
}
