// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EmotionUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UEmotionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Hello;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Happy;
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_37;

	UFUNCTION()
	void OnClickHello();
	UFUNCTION()
	void OnClickHappy();
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowEmotionUI;

	UFUNCTION()
	void PlayShowAnim(bool bIsForward);
};
