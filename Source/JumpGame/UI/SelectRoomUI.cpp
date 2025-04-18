// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectRoomUI.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void USelectRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GoBackWait->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickGoBackWait);
	Btn_SelectComplete->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickSelectComplete);
	
	Btn_AllMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickAllMap);
	Btn_OriginMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickOriginMap);
	Btn_CustomMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickCustomMap);
}

void USelectRoomUI::OnClickGoBackWait()
{
	// 맵 선택 취소
	RemoveFromParent();
}

void USelectRoomUI::OnClickSelectComplete()
{
	// TODO: 맵 정보 넘겨주기
	// 선택완료 됐으니 팝업창 지우자
	RemoveFromParent();
}

void USelectRoomUI::OnClickAllMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(0);
}

void USelectRoomUI::OnClickOriginMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(1);
}

void USelectRoomUI::OnClickCustomMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(2);
}
