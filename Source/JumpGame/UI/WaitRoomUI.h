// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectRoomUI.h"
#include "Blueprint/UserWidget.h"
#include "WaitRoomUI.generated.h"

/*
여기는 대기방 (유저들이 입장해서 대기하는 방)
-> 서버가 게임 시작 버튼 클릭 시 레벨전환
 */
UCLASS()
class JUMPGAME_API UWaitRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UJumpGameInstance* GI;
	
	// 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameStart;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SelectMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromLobby;

	// 클릭시 실행될 함수
	UFUNCTION()
	void OnClickGameStart();
	UFUNCTION()
	void OnClickSelectMap();
	UFUNCTION()
	void OnClickBackFromLobby();

	// 팝업으로 띄울 UI
	UPROPERTY(editanywhere)
	TSubclassOf<class USelectRoomUI> SelectRoomFactory;

	// 유저가 들어오면 업데이트 될 이미지 배열
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user3;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user4;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user5;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user6;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user7;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user8;

	// 이미지 업데이트 함수
	void UpdateWaitImage();
};
