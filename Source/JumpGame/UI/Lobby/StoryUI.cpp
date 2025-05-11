// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryUI.h"

#include "Animation/UMGSequencePlayer.h"
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

	StoryArray.Add(FText::FromString(TEXT("나레이션:\n아직 두 발로 멀리 뛰지 못하던 시절,\n깨구락이는 물을 무서워했다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마:\n오늘은 너랑 특별한 곳에 가보자.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n엄마는 조용히 깨구락의 손을 잡고,\n마을 외곽의 ‘달빛 연못’으로 향했다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n그곳은 달빛이 은빛 천처럼 퍼지는\n신비로운 연못이었다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마:\n이 물은 네 외할머니가 처음 발을 담근 곳이야.\n그리고 내가 물을 두려워하지 않게 된 곳이기도 해.")));
	StoryArray.Add(FText::FromString(TEXT("엄마:\n오늘은 네 차례란다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n연못 한가운데, 달수련이 천천히 빛을 냈다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n은색 물고기 한 마리가 잎배 밑을 지나간다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마:\n이건 ‘처음의 돌’이란다.\n우리 가문에선 이걸 던지며 용기를 내지.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n(깨구락이가 손을 떨며 조약돌을 던진다)")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n물속에 퍼지는 동심원.\n불안이 서서히 풀려나간다.")));
	StoryArray.Add(FText::FromString(TEXT("엄마:\n물은 널 삼키는 게 아니야.\n너를 안아주는 거지.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n깨구락은 숨을 들이쉬고,\n몸을 물 위로 띄웠다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n보름달이 떠오르고,\n모든 것이 은빛으로 물든다.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n(깨구락이 수영을 한다)")));
	StoryArray.Add(FText::FromString(TEXT("깨구락:(헐떡이며)\n저 어때요! 허우적… 물을… 마셨지만…")));
	StoryArray.Add(FText::FromString(TEXT("엄마:\n너는 정말 잘하고 있어.")));
	StoryArray.Add(FText::FromString(TEXT("나레이션:\n연못 바닥엔 오래된 조약돌들이 있었다.\n그 위에, 깨구락이의 첫 용기도 내려앉았다.")));
	StoryArray.Add(FText::FromString(TEXT("깨구락:\n나… 해냈어?")));
	StoryArray.Add(FText::FromString(TEXT("엄마:\n달빛 연못은 기억할 거야.\n네가 처음 용기를 낸 오늘을.")));
	StoryArray.Add(
		FText::FromString(TEXT("나레이션:\n그날 이후, 달빛 연못은 두려움을 이긴 기억이자,\n엄마의 손길, 첫 용기, 가문의 전통,\n그리고… 달의 축복이 되었다.")));
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
		TextBlock_Mom->SetText(FText::FromString(TEXT("엄마")));
		TextBlock_Mom->SetVisibility(ESlateVisibility::Visible);
	}
	else if (NewSpeakerName.Contains(TEXT("깨구락")))
	{
		TextBlock_Baby->SetText(FText::FromString(TEXT("깨구락")));
		TextBlock_Baby->SetVisibility(ESlateVisibility::Visible);
	}
	// "나레이션"의 경우 TextBlock을 사용하지 않음 

	// 대화 내용
	TextBlock_Talk->SetText(FText::FromString(DialogText));

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
		// 어두워지는 애니메이션
		RemoveFromParent();
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
			TextBlock_Talk->SetText(FText::FromString(TEXT("<< 이야기가 끝났습니다 >>")));
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

			UpdateSpeakerAndText(StoryArray[CurrentStoryIndex]);
		});
	}
}
