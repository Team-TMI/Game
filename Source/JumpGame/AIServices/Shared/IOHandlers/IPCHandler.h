// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*
 * 이 오류는 RHITransientResourceAllocator.h 파일에서 max 매크로 충돌이 발생했기 때문에 생긴 것입니다. Visual Studio나 Windows 헤더에서는 흔히 <windows.h>가 max와 min을 매크로로 정의하기 때문에, C++ 표준 라이브러리의 std::max / std::min과 충돌하게 됩니다.
 */
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

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
	
	FString PipeName;

	int32 BufferSize = BUFFER_SIZE;
	char Buffer[BUFFER_SIZE];

	size_t MaxCacheSize = BUFFER_SIZE * 10;
	uint8 CachedBuffer[BUFFER_SIZE * 10];
	size_t CachedLength = 0;
	
	HANDLE Pipe;
};
