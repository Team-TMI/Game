﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryUI.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "JumpGame/Utils/FastLogger.h"

void UStoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LobbyController = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (LobbyController)
	{
		LobbyController->OnNextChatTriggered.AddDynamic(this, &UStoryUI::OnNextChatTriggered);
	}

	InitializeStory();
	DisplayInitialStory();

	if (TextBlock_Mom && TextBlock_Baby)
	{
		TextBlock_Mom->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_Baby->SetVisibility(ESlateVisibility::Hidden);
	}

	ChatBoxImage->SetRenderOpacity(0.f);
}

// void UStoryUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
// {
// 	Super::NativeTick(MyGeometry, InDeltaTime);
// }

void UStoryUI::InitializeStory()
{
	bIsEnd = false;

	CurrentStoryIndex = 0;
	StoryArray.Empty();

	bIsSpeakerChanged = false;
	SpeakerName = TEXT("나레이션");
}

void UStoryUI::DisplayInitialStory()
{
	if (StoryArray.IsValidIndex(CurrentStoryIndex) && TextBlock_Talk)
	{
		UUMGSequencePlayer* SequencePlayer{PlayAnimation(ChatAppear, 0.5f, 1, EUMGSequencePlayMode::Forward, 0.3f)};
		SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) {
			if (!IsValid(this))
			{
				return;
			}

			CheckNextSpeaker(StoryArray[CurrentStoryIndex + 1]);
			UpdateSpeakerAndText(StoryArray[CurrentStoryIndex]);
		});
	}
}

void UStoryUI::UpdateSpeakerAndText(const FText& FullStoryLine)
{
	if (!TextBlock_Talk || !TextBlock_Mom || !TextBlock_Baby)
	{
		return;
	}

	FString StoryString{FullStoryLine.ToString()};
	FString NewSpeakerName;
	FString DialogText;

	// 화자와 대사 분리 (':' 기준)
	int32 ColonIndex;

	if (StoryString.FindChar(TEXT(':'), ColonIndex))
	{
		// 콜론 앞부분
		NewSpeakerName = StoryString.Left(ColonIndex).TrimStartAndEnd();
		// 콜론 뒷부분
		DialogText = StoryString.Mid(ColonIndex + 1).TrimStart();
	}

	TextBlock_Mom->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_Baby->SetVisibility(ESlateVisibility::Hidden);

	if (NewSpeakerName.Contains(TEXT("엄마")))
	{
		TextBlock_Mom->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(MomTalk, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
	else if (NewSpeakerName.Contains(TEXT("깨구락")))
	{
		TextBlock_Baby->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(BabyTalk, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
	// "나레이션"의 경우 TextBlock을 사용하지 않음 

	// 대화 내용
	TextBlock_Talk->SetText(FText::FromString(DialogText));
	// 대화 내용 서서히 등장
	PlayAnimation(TextAppear, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

	if (StoryArray.IsValidIndex(CurrentStoryIndex + 1))
	{
		CheckNextSpeaker(StoryArray[CurrentStoryIndex + 1]);
	}
}


void UStoryUI::CheckNextSpeaker(const FText& FullStoryLine)
{
	if (!TextBlock_Talk || !TextBlock_Mom || !TextBlock_Baby)
	{
		return;
	}

	FString StoryString{FullStoryLine.ToString()};
	FString NewSpeakerName;
	int32 ColonIndex;

	if (StoryString.FindChar(TEXT(':'), ColonIndex))
	{
		NewSpeakerName = StoryString.Left(ColonIndex).TrimStartAndEnd();
	}

	if (SpeakerName != NewSpeakerName)
	{
		SpeakerName = NewSpeakerName;
		bIsSpeakerChanged = true;
	}
}

void UStoryUI::OnNextChatTriggered()
{
	if (bIsEnd)
	{
		StoryEnd();
		return;
	}

	TextBlock_Talk->SetText(FText::FromString(TEXT("")));

	// 다음 대사 인덱스 증가
	++CurrentStoryIndex;

	if (StoryArray.IsValidIndex(CurrentStoryIndex))
	{
		if (TextBlock_Talk)
		{
			if (bIsSpeakerChanged)
			{
				TextBlock_Mom->SetVisibility(ESlateVisibility::Hidden);
				TextBlock_Baby->SetVisibility(ESlateVisibility::Hidden);

				bIsSpeakerChanged = false;
				ChatAppearAnimation();
			}
			else
			{
				UpdateSpeakerAndText(StoryArray[CurrentStoryIndex]);
			}
		}
	}
	// 이야기가 끝났을 때
	else
	{
		bIsEnd = true;

		if (TextBlock_Talk)
		{
			TextBlock_Talk->SetText(FText::FromString(TEXT("- 이야기가 끝났습니다 -")));
		}
		if (TextBlock_Mom)
		{
			TextBlock_Mom->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TextBlock_Baby)
		{
			TextBlock_Baby->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UStoryUI::ChatAppearAnimation()
{
	if (ChatAppear)
	{
		UUMGSequencePlayer* SequencePlayer{PlayAnimation(ChatAppear, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f)};
		SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) {
			if (!IsValid(this))
			{
				return;
			}

			if (StoryArray.IsValidIndex(CurrentStoryIndex))
			{
				UpdateSpeakerAndText(StoryArray[CurrentStoryIndex]);
			}
		});
	}
}

void UStoryUI::StoryEnd()
{
	// 어두워지는 애니메이션
	RemoveFromParent();
}
