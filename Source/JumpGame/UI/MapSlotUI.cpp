// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSlotUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void UMapSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_MapSlot->OnClicked.AddDynamic(this, &UMapSlotUI::OnBtnMapSlotClicked);
}

void UMapSlotUI::Init(const FString& InMapFullPath, const FString& InMapName)
{
	MapName = InMapName;
	MapFullPath = InMapFullPath;
	// 파일 이름이 너무 길면 자르자
	if (Text_MapName)
	{
		FString ShortenedMapName = MapName;
		if (ShortenedMapName.Len() > 10)
		{
			ShortenedMapName = ShortenedMapName.Left(10) + TEXT("...");
		}
		Text_MapName->SetText(FText::FromString(ShortenedMapName));
	}
}

void UMapSlotUI::OnBtnMapSlotClicked()
{
	OnClicked.ExecuteIfBound(this);
	FFastLogger::LogScreen(FColor::Green, TEXT("MapSlot Clicked: %s"), *MapName);
}

