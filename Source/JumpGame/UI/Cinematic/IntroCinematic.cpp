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
