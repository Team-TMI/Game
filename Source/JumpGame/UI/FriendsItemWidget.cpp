// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendsItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"


void UFriendsItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());

	Btn_Invite->OnClicked.AddDynamic(this, &UFriendsItemWidget::OnClickInvite);
}

void UFriendsItemWidget::OnClickInvite()
{
	if (GI)
	{
		const auto& Friends = GI->GetFilteredFriendList();
		if (Friends.IsValidIndex(FriendIndex))
		{
			GI->InviteFriendToSession(Friends[FriendIndex].SteamId);
		}
	}
}

void UFriendsItemWidget::SetFriendInfo(const FSteamFriendData& FriendData)
{
	if (Text_FriendName)
	{
		Text_FriendName->SetText(FText::FromString(FriendData.DisplayName));
	}

	FString SteamId = FriendData.SteamId;
}
