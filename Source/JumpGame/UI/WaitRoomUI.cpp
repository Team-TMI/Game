// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitRoomUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Utils/FastLogger.h"

class UTextBlock;

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
	// 세션파괴해야됨!
	GI->LeaveSession(true);
}

void UWaitRoomUI::UpdateWaitPlayer()
{
	if (!GI) return;

	// 이미지 배열
	TArray<UImage*> UpdateImgs = { Image_user1, Image_user2, Image_user3, Image_user4, Image_user5, Image_user6, Image_user7, Image_user8 };
	// 플레이어 이름 배열
	TArray<UTextBlock*> UpdateNames = { Text_Player1, Text_Player2, Text_Player3, Text_Player4, Text_Player5, Text_Player6, Text_Player7, Text_Player8 };
	
	TMap<FString, FPlayerInfo> Info = GI->GetPlayerInfo();
	FString Key;
	
	for (int i = 0; i < UpdateImgs.Num(); i++)
	{
		for (auto& it : Info)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stored Key : %s"), *it.Key);
			if (it.Value.PlayerID == i)
			{
				// 플레이어 인덱스와 같은 이미지, 이름을 보이게 하자
				UpdateImgs[i]->SetRenderOpacity(1);
				UpdateNames[i]->SetText(FText::FromString(it.Value.PlayerName));
				UpdateNames[i]->SetRenderOpacity(1);
			}
		}
	}
}
