// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPage.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UVictoryPage::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GoLobby->OnClicked.AddDynamic(this, &UVictoryPage::OnClickGoLobby);
}

void UVictoryPage::OnClickGoLobby()
{
	// 각자 알아서 로비로 돌아가자
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		// 알아보기: 서버나, 클라이언트나 상관없음?
		PC->ClientTravel(TEXT("/Game/Maps/WaitRoomLevel"), TRAVEL_Absolute);
	}
}

void UVictoryPage::SetVictoryPlayerName(FString PlayerName)
{
	// WinnerPlayer 이름 업데이트
	Text_PlayerName->SetText(FText::FromString(PlayerName));
}
