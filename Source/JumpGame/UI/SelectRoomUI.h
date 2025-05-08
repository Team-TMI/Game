// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectRoomUI.generated.h"

/*
 */
UCLASS()
class JUMPGAME_API USelectRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	// 맵 선택 창 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoBackWait;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SelectComplete;

	// 맵 종류 선택 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AllMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_OriginMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CustomMap;

	// 버튼 클릭시 실행 함수
	UFUNCTION()
	void OnClickGoBackWait();
	UFUNCTION()
	void OnClickSelectComplete();
	UFUNCTION()
	void OnClickAllMap();
	UFUNCTION()
	void OnClickOriginMap();
	UFUNCTION()
	void OnClickCustomMap();
	
	// 맵 리스트 관련
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_SR;
};
