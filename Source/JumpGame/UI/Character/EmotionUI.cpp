// Fill out your copyright notice in the Description page of Project Settings.


#include "EmotionUI.h"

#include "Components/Button.h"
#include "JumpGame/Utils/FastLogger.h"

void UEmotionUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Hello->OnClicked.AddDynamic(this, &UEmotionUI::OnClickHello);
	Btn_Happy->OnClicked.AddDynamic(this, &UEmotionUI::OnClickHappy);
}

void UEmotionUI::OnClickHello()
{
	FFastLogger::LogScreen(FColor::Red, TEXT("Hello 클릭"));
}

void UEmotionUI::OnClickHappy()
{
	FFastLogger::LogScreen(FColor::Red, TEXT("Happy 클릭"));
}

void UEmotionUI::PlayShowAnim(bool bIsForward)
{
	if (!ShowEmotionUI) return;

	if (bIsForward)
	{
		PlayAnimation(ShowEmotionUI);
	}
	else
	{
		PlayAnimation(ShowEmotionUI, 0, 1, EUMGSequencePlayMode::Reverse);
	}
}
