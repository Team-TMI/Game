// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <queue>

#include "CoreMinimal.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "IOHandlerInitInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct JUMPGAME_API FIOHandlerInitInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(Blueprintable)
	FString ServerUrl = TEXT("");
	UPROPERTY(Blueprintable)
	FString ServerProtocol = TEXT("");
	UPROPERTY(Blueprintable)
	FString PipeName = TEXT("");

	std::map<EMessageType, std::queue<FMessageUnion>>* MessageQueue = nullptr;

	FIOHandlerInitInfo(const FString& InServerUrl = TEXT(""), const FString& InServerProtocol = TEXT(""), const FString& InPipeName = TEXT(""))
		: ServerUrl(InServerUrl), ServerProtocol(InServerProtocol), PipeName(InPipeName)
	{
	}
};
