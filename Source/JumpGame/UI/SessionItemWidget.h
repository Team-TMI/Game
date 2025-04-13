// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API USessionItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Join;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RoomInfo;

	int32 RoomIdx;
	
	UFUNCTION()
	void OnClickJoin();
	void SetInfo(int32 Idx, FString Info);
};
