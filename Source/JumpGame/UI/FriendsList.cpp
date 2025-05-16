// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendsList.h"

#include "FriendsItemWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"

void UFriendsList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GI)
	{
		GI->OnFriendListUpdated.AddDynamic(this, &UFriendsList::UpdateFriendList);
	}

	Btn_Refresh->OnClicked.AddDynamic(this, &UFriendsList::OnClickRefresh);

	// 초기화
	Init();
}

void UFriendsList::Init()
{
	ScrollBox_Friend->ClearChildren();
	FriendsItemPool.Empty();

	for (int32 i = 0; i < MaxFriendCount; i++)
	{
		// Friend Widget 생성
		UFriendsItemWidget* FriendsItemWidget = CreateWidget<UFriendsItemWidget>(this, FriendsItemWidgetFactory);
		FriendsItemWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		// 배열에 저장
		FriendsItemPool.Add(FriendsItemWidget);

		// 스크롤 박스에 추가
		ScrollBox_Friend->AddChild(FriendsItemWidget);
	}
}

void UFriendsList::UpdateFriendList(const TArray<FSteamFriendData>& FriendDataArray)
{
	for (int i = 0; i < FriendsItemPool.Num(); ++i)
	{
		if (i < FriendDataArray.Num())
		{
			FriendsItemPool[i]->SetFriendInfo(FriendDataArray[i]); // 데이터 바인딩
			FriendsItemPool[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			FriendsItemPool[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UFriendsList::OnClickRefresh()
{
	GI->GetSteamFriends();
}

void UFriendsList::OnFindComplete(const FSteamFriendData& Data)
{
}

void UFriendsList::PlayShowAnim(bool bIsForward)
{
	if (!FriendListAnim) return;

	if (bIsForward)
	{
		// 앞으로 재생
		PlayAnimation(FriendListAnim);
		bIsInViewport = true;
	}
	else
	{
		// 역재생
		PlayAnimation(FriendListAnim, 0, 1, EUMGSequencePlayMode::Reverse);
		bIsInViewport = false;
	}
}
