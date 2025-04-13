// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"

void USessionItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Join->OnClicked.AddDynamic(this,&USessionItemWidget::OnClickJoin);
}

void USessionItemWidget::OnClickJoin()
{
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	GI->JoinOtherSession(RoomIdx);
}

void USessionItemWidget::SetInfo(int32 Idx, FString Info)
{
	// 현재 몇번째 방인지?
	RoomIdx = Idx;
	// 방 정보 받기 (업데이트)
	Text_RoomInfo->SetText(FText::FromString(Info));
}
