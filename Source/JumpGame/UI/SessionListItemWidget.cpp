// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"

void USessionListItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Join->OnClicked.AddDynamic(this,&USessionListItemWidget::OnClickJoin);
}

void USessionListItemWidget::OnClickJoin()
{
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	GI->JoinOtherSession(RoomIdx);
}

void USessionListItemWidget::SetInfo(int32 Idx, FString Info)
{
	// 현재 몇번째 방인지?
	RoomIdx = Idx;
	// 방 정보 받기 (업데이트)
	Text_RoomInfo->SetText(FText::FromString(Info));
}
