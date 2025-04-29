// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundQuizUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Props/ObstacleProp/SoundQuizProp/VoiceRecorderComponent.h"
#include "JumpGame/Utils/FastLogger.h"

// NOTE: 바인딩은 한번만 실행되어야함 NativeConstruct() 에서 실행시 여러번 호출되어 에러 발생
void USoundQuizUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Btn_VoiceSend->OnClicked.AddDynamic(this, &USoundQuizUI::OnClickVoiceSend);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
}

void USoundQuizUI::OnClickVoiceSend()
{
	// 녹음을 끝내고 AI에게 녹음 파일을 보내자
	if (VoiceRecorderComponent.IsValid())
	{
		VoiceRecorderComponent->StopRecording();
		FFastLogger::LogConsole(TEXT("녹음끝@@@@@@@@@@@@@@@@@@@@@@@"));
	}

	// 5초 안에 안누르면 자동으로 보내자
	// GetWorld()->GetTimerManager().SetTimer(RecordTimer, this, USoundQuizUI::OnClickVoiceSend, 5.0f, false);
}

void USoundQuizUI::UpdateFromResponse(uint32 Similarity, FString MessageStr)
{
	FText SimilarityText = FText::AsNumber(Similarity);
	FText MessageText = FText::FromString(MessageStr);
	
	// 받아와서 업데이트 하자
	Text_Similarity->SetText(SimilarityText);
	Text_MessageStr->SetText(MessageText);
}

void USoundQuizUI::SetVoiceRecorderComponent(UVoiceRecorderComponent* VoiceRecorderComp)
{
	if (!VoiceRecorderComp) return;

	VoiceRecorderComponent = VoiceRecorderComp;
	VoiceRecorderComponent->GetOnStopRecording().AddDynamic(this, &USoundQuizUI::OnClickVoiceSend);
}


