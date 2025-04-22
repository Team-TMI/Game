// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomUI.h"

#include "SessionListItemWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UClientRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 마우스 활성화
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());

	// WidgetSwitcher (0)
	// 메인 화면 버튼 클릭
	Btn_GoFind->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoFindRoom);
	Btn_GoCreateMap->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoCreateMap);
	Btn_GoFrogPass->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoFrogPass);
	Btn_GoSettings->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoSettings);
	Btn_GoCredit->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoCredit);
	Btn_GoGameEnd->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoGameEnd);

	// WidgetSwitcher (1)
	// 방 만들기 화면 버튼 클릭
	Btn_GoCreate->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickGoCreateRoom);
	Btn_Create->OnClicked.AddDynamic(this, &UClientRoomUI::CreateSession);
	Btn_BackFromCreate->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickBack);
	// 슬라이더 값 변경될때 호출되는 함수
	Slider_PlayerCnt->OnValueChanged.AddDynamic(this, &UClientRoomUI::OnValueChanged);
	// 비밀번호 설정 함수
	CheckBox_Lock->OnCheckStateChanged.AddDynamic(this, &UClientRoomUI::OnClickCheckBox);
	
	// session 검색 관련 ui 버튼 클릭
	Btn_Find->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickFind);
	Btn_BackFromFind->OnClicked.AddDynamic(this, &UClientRoomUI::OnClickBackFromFind);
	GI->OnFindComplete.BindUObject(this, &UClientRoomUI::OnFindComplete);
}


void UClientRoomUI::OnClickGoFindRoom()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);

	// 세션 검색 화면 넘어갈때 자동으로 한번은 세션을 검색해주자
	OnClickFind();
}

void UClientRoomUI::OnClickGoCreateMap()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("SkyeTestMap"));
}

void UClientRoomUI::OnClickGoFrogPass()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

void UClientRoomUI::OnClickGoSettings()
{
	
}

void UClientRoomUI::OnClickGoCredit()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
}

void UClientRoomUI::OnClickGoGameEnd()
{
	
}

void UClientRoomUI::OnClickGoCreateRoom()
{
	// UI를 팝업으로 띄우자
	CanvasCreateRoom->SetVisibility(ESlateVisibility::Visible);
}

void UClientRoomUI::CreateSession()
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

void UClientRoomUI::OnClickCheckBox(bool bIsChecked)
{
	Edit_Password->SetIsEnabled(bIsChecked);
}

void UClientRoomUI::OnValueChanged(float Value)
{
	// value값을 text값으로 변경후 세팅
	Text_PlayerCnt->SetText(FText::AsNumber(Value));
}

void UClientRoomUI::OnClickFind()
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

void UClientRoomUI::OnClickBack()
{
	// UI를 팝업으로 띄우자
	CanvasCreateRoom->SetVisibility(ESlateVisibility::Hidden);
}

void UClientRoomUI::OnFindComplete(int32 Idx, FString Info)
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
		USessionListItemWidget* Item = CreateWidget<USessionListItemWidget>(GetWorld(), SessionItemFactory);
		// 만들어진 SessionItem을 Scroll_RoomList에 자식으로!
		Scroll_RoomList->AddChild(Item);
		// 만들어진 SessionItem의 Text 내용을 변경하고, idx 전달하자
		Item->SetInfo(Idx, Info);
	}
}

void UClientRoomUI::OnClickBackFromFind()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}
