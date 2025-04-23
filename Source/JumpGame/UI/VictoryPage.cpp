// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPage.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

void UVictoryPage::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GoLobby->OnClicked.AddDynamic(this, &UVictoryPage::OnClickGoLobby);
}

void UVictoryPage::OnClickGoLobby()
{
	// 각자 알아서 혼자! 로비로 돌아가자
	// 알아보기: 서버나, 클라이언트나 상관없이 "혼자"이동?
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC->HasAuthority())
	{
		PC->ClientTravel(TEXT("/Game/Maps/WaitRoomLevel"), TRAVEL_Absolute);
		FFastLogger::LogConsole(TEXT("클라이언트, 로비로 이동합니다~~~"));
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(),TEXT("WaitRoomLevel"));
		FFastLogger::LogConsole(TEXT("서버, 로비로 이동합니다~~~"));
	}

	// 이동하면 나는 지우자
	RemoveFromParent();
}

void UVictoryPage::SetVictoryPlayerName(FString PlayerName)
{
	// WinnerPlayer 이름 업데이트
	Text_PlayerName->SetText(FText::FromString(PlayerName));
}
