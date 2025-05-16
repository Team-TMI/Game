// Fill out your copyright notice in the Description page of Project Settings.


#include "BottomNaviBarUI.h"

#include "FriendsList.h"
#include "Components/Button.h"

void UBottomNaviBarUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Store->OnClicked.AddDynamic(this, &UBottomNaviBarUI::OnClickStore);
	Btn_FrogPass->OnClicked.AddDynamic(this, &UBottomNaviBarUI::OnClickFrogPass);
	Btn_Friend->OnClicked.AddDynamic(this, &UBottomNaviBarUI::OnClickFriend);

	FriendsList = CreateWidget<class UFriendsList>(GetWorld(), FriendsListClass);
	if (FriendsList)
	{
		FriendsList->AddToViewport();
	}
}

void UBottomNaviBarUI::OnClickStore()
{
}

void UBottomNaviBarUI::OnClickFrogPass()
{
}

void UBottomNaviBarUI::OnClickFriend()
{
	if (!bIsOnViewport)
	{
		// 띄우고
		FriendsList->PlayShowAnim(true);
		bIsOnViewport = true;
	}
	else
	{
		// 지우고
		FriendsList->PlayShowAnim(false);
		bIsOnViewport = false;
	}
}
