// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPageUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

void UVictoryPageUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GoLobby->OnClicked.AddDynamic(this, &UVictoryPageUI::OnClickGoLobby);
}

void UVictoryPageUI::OnClickGoLobby()
{
	// 서버가 나가면 다같이 나가기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC->HasAuthority())
	{
		// 혼자나가면 연결끊기
		PC->ClientTravel(TEXT("/Game/Maps/ClientRoomLevel?closed"), TRAVEL_Absolute);
	}
	else
	{
		GetWorld()->ServerTravel(TEXT("/Game/Maps/ClientRoomLevel?listen"));
		FLog::Log("Server Leaving Game, 로비로 이동");
	}

	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	GI->LeaveSession(true);

	// 이동하면 나는 지우자
	RemoveFromParent();
}

void UVictoryPageUI::SetVictoryPlayerName(FString PlayerName)
{
	// WinnerPlayer 이름 업데이트
	Text_PlayerName->SetText(FText::FromString(PlayerName));
}
