// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"

#include "SessionItemWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 마우스 활성화
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// 버튼 클릭할때 호출되는 함수
	Btn_Create->OnClicked.AddDynamic(this, &ULobbyUI::CreateSession);
	Btn_BackFromCreate->OnClicked.AddDynamic(this, &ULobbyUI::OnClickBack);
	// 슬라이더 값 변경될때 호출되는 함수
	Slider_PlayerCnt->OnValueChanged.AddDynamic(this, &ULobbyUI::OnValueChanged);

	// main ui 버튼 클릭
	Btn_GoCreate->OnClicked.AddDynamic(this, &ULobbyUI::OnClickGoCreateRoom);
	Btn_GoFind->OnClicked.AddDynamic(this, &ULobbyUI::OnClickGoFindRoom);

	// session 검색 관련 ui 버튼 클릭
	Btn_Find->OnClicked.AddDynamic(this, &ULobbyUI::OnClickFind);
	Btn_BackFromFind->OnClicked.AddDynamic(this, &ULobbyUI::OnClickBack);
	GI->OnFindComplete.BindUObject(this, &ULobbyUI::OnFindComplete);
}

void ULobbyUI::OnClickGoCreateRoom()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyUI::OnClickGoFindRoom()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);

	// 세션 검색 화면 넘어갈때 자동으로 한번은 세션을 검색해주자
	OnClickFind();
}

void ULobbyUI::CreateSession()
{
	// 방 제목 가져오기
	FString RoomName = Edit_RoomName->GetText().ToString();
	// 만약 방 제목 없으면 나가자
	if (RoomName.IsEmpty()) return;
	
	// 인원 수
	int32 PlayerCnt = Slider_PlayerCnt->GetValue();
	// 생성
	GI->CreateMySession(RoomName, PlayerCnt);
}

void ULobbyUI::OnValueChanged(float Value)
{
	// value값을 text값으로 변경후 세팅
	Text_PlayerCnt->SetText(FText::AsNumber(Value));
}

void ULobbyUI::OnClickFind()
{
	// 스크롤 룸리스트의 자식들 다 지우자 (초기화, 중복 방지)
	Scroll_RoomList->ClearChildren();
	// 그 다음에 검색을 시작
	GI->FindOtherSession();

	// 찾는 도중엔 버튼을 비활성화
	// 검색 버튼 문구 바꿔주고
	Text_BtnFind->SetText(FText::FromString("Searching..."));
	// 검색 버튼을 비활성화
	Btn_Find->SetIsEnabled(false);
}

void ULobbyUI::OnClickBack()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyUI::OnFindComplete(int32 Idx, FString Info)
{
	// 만약에 idx가 -1이면
	if (Idx==-1)
	{
		// 검색 버튼 내용 다시 find로
		Text_BtnFind->SetText(FText::FromString("FIND"));
		// 검색 버튼을 활성화
		Btn_Find->SetIsEnabled(true);
	}
	// 그렇지 않으면
	else
	{
		// SessionItem을 하나 만들자
		USessionItemWidget* Item = CreateWidget<USessionItemWidget>(GetWorld(), sessionItemFactory);
		// 만들어진 SessionItem을 Scroll_RoomList에 자식으로!
		Scroll_RoomList->AddChild(Item);
		// 만들어진 SessionItem의 Text 내용을 변경하고, idx 전달하자
		Item->SetInfo(Idx, Info);
	}
}
