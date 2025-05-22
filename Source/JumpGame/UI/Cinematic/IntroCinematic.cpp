// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroCinematic.h"

#include "FileMediaSource.h"
#include "MediaPlayer.h"

void UIntroCinematic::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MediaPlayer && MediaSource)
	{
		MediaPlayer->OpenSource(MediaSource);
		MediaPlayer->Play();
	}
}

void UIntroCinematic::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
	{
		if (MediaPlayer->SupportsRate(3.0f, true))
		{
			MediaPlayer->SetRate(3.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("배속 재생이 지원되지 않습니다."));
		}
	}
}
