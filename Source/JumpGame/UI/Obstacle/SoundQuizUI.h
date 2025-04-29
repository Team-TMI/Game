// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoundQuizUI.generated.h"

class UVoiceRecorderComponent;
/**
 * 
 */
UCLASS()
class JUMPGAME_API USoundQuizUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_VoiceSend;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Similarity;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MessageStr;

	UFUNCTION()
	void OnClickVoiceSend();
	void UpdateFromResponse(uint32 Similarity, FString MessageStr);

	FTimerHandle RecordTimer;

	// 녹음 컴포넌트 동작
	TWeakObjectPtr<UVoiceRecorderComponent> VoiceRecorderComponent = nullptr;

	void SetVoiceRecorderComponent(UVoiceRecorderComponent* VoiceRecorderComp);
};
