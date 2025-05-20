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
	if (Text_MapName)
	{
		Text_MapName->SetText(FText::FromString(MapName));
	}
}

void UMapSlotUI::OnBtnMapSlotClicked()
{
	OnClicked.ExecuteIfBound(this);
	FFastLogger::LogScreen(FColor::Green, TEXT("MapSlot Clicked: %s"), *MapName);
}

