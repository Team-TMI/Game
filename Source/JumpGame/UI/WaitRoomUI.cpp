// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitRoomUI.h"

#include "Components/Button.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Utils/FastLogger.h"

void UWaitRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	
	Btn_GameStart->OnClicked.AddDynamic(this, &UWaitRoomUI::OnClickGameStart);
	Btn_SelectMap->OnClicked.AddDynamic(this, &UWaitRoomUI::OnClickSelectMap);
	Btn_BackFromLobby->OnClicked.AddDynamic(this, &UWaitRoomUI::OnClickBackFromLobby);
}

void UWaitRoomUI::OnClickGameStart()
{
	// 서버만 게임을 시작할 수 있다
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC->HasAuthority())
	{
		// TODO: 선택한 게임레벨로 이동하게하자
		GetWorld()->ServerTravel(TEXT("/Game/Maps/InGameLevel?listen"));
		UE_LOG(LogTemp, Warning, TEXT("게임시작"));
	}
}

void UWaitRoomUI::OnClickSelectMap()
{
	// 서버만 맵을 고를 수 있다
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC->HasAuthority())
	{
		// 맵선택 UI가 뜨게 하자
		USelectRoomUI* SelectRoomUI = CreateWidget<USelectRoomUI>(GetWorld(), SelectRoomFactory);
		SelectRoomUI->AddToViewport();
	}
}

void UWaitRoomUI::OnClickBackFromLobby()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC->HasAuthority())
	{
		PC->ClientTravel(TEXT("/Game/Maps/ClientRoomLevel"), TRAVEL_Absolute);
	}
	else
	{
		GetWorld()->ServerTravel(TEXT("/Game/Maps/ClientRoomLevel?listen"));
		FLog::Log("Server Leaving Session");
	}
	
	GI->LeaveSession(true);
}

void UWaitRoomUI::UpdateWaitImage()
{
	if (!GI) return;

	// 이미지 배열
	TArray<UImage*> UpdateImgs = { Image_user1, Image_user2, Image_user3, Image_user4, Image_user5, Image_user6, Image_user7, Image_user8 };

	/*
	TMap<FString, FPlayerInfo> Info = GI->GetPlayerInfo();
	FString Key;
	
	for (int i = 0; i < UpdateImgs.Num(); i++)
	{
		for (auto& it : Info)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stored Key : %s"), *it.Key);
			if (it.Value.PlayerID == i)
			{
				// 플레이어 인덱스와 같은 이미지를 보이게 하자
				UpdateImgs[i]->SetRenderOpacity(1);
			}
		}
	}
	*/
}
