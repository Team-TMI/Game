// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "IOHandlerInterface.h"
#include "JumpGame/AIServices/Shared/Message.h"

#define BUFFER_SIZE 1460
#define PIPE_PREFIX TEXT(R"(\\.\pipe\)")

class JUMPGAME_API FIPCHandler : public IIOHandlerInterface, public TSharedFromThis<FIPCHandler>
{
public:
	FIPCHandler();

	virtual bool Init(const FIOHandlerInitInfo& InitInfo, std::map<EMessageType, std::queue<FMessageUnion>>* InMessageQueuePtr) override;
	virtual bool SendGameMessage(const FMessageUnion& Message) override;
	virtual bool ReceiveMessage() override;
	
	virtual ~FIPCHandler() override;

private:
	bool ParseMessage(FMessageUnion& Message, DWORD BytesRead);
	
	FString PipeName = TEXT("");

	int32 BufferSize = BUFFER_SIZE;
	char Buffer[BUFFER_SIZE] = { 0 };

	size_t MaxCacheSize = BUFFER_SIZE * 10;
	uint8 CachedBuffer[BUFFER_SIZE * 10] = { 0 };
	size_t CachedLength = 0;
	
	void* Pipe = nullptr;
};
