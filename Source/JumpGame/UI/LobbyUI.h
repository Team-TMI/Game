// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

/*
여기는 대기방 (진짜 로비)
 */
UCLASS()
class JUMPGAME_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UJumpGameInstance* GI;
	
public:
	// 위젯 스위처
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	
	// 세션 메인 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoCreate;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoFind;
	// 눌렀을때 호출되는 함수
	UFUNCTION()
	void OnClickGoCreateRoom();
	UFUNCTION()
	void OnClickGoFindRoom();
	
	// 세션 생성 관련
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Edit_RoomName;
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_PlayerCnt;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerCnt;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Create;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromCreate;
	
	// 버튼 눌렀을 때 호출될 함수
	UFUNCTION()
	void CreateSession();
	
	// 슬라이더
	UFUNCTION()
	void OnValueChanged(float Value);
	
	// 세션 검색 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Find;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromFind;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_RoomList;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_BtnFind;
	UPROPERTY(editanywhere)
	TSubclassOf<class USessionItemWidget> sessionItemFactory;
	
	// 함수
	UFUNCTION()
	void OnClickFind();
	UFUNCTION()
	void OnClickBack();

	// 델리게이트 함수
	void OnFindComplete(int32 Idx,FString Info);
};
