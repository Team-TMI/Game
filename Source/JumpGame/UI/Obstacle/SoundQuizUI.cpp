// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundQuizUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USoundQuizUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_VoiceSend->OnClicked.AddDynamic(this, &USoundQuizUI::OnClickVoiceSend);
}

void USoundQuizUI::OnClickVoiceSend()
{
	// 녹음을 끝내고 AI에게 녹음 파일을 보내자
}

void USoundQuizUI::UpdateFromResponse(uint32 Similarity, FString MessageStr)
{
	FText SimilarityText = FText::AsNumber(Similarity);
	FText MessageText = FText::FromString(MessageStr);
	
	// 받아와서 업데이트 하자
	Text_Similarity->SetText(SimilarityText);
	Text_MessageStr->SetText(MessageText);
}


