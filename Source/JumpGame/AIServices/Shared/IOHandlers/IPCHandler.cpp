// Fill out your copyright notice in the Description page of Project Settings.


#include "IPCHandler.h"

#include "JumpGame/Utils/FastLogger.h"

FIPCHandler::FIPCHandler()
{
}

void FIPCHandler::Init(const FIOHandlerInitInfo& InitInfo, 
	std::map<EMessageType, std::queue<FMessageUnion>>* InMessageQueuePtr)
{
	PipeName = PIPE_PREFIX + InitInfo.PipeName;
	MessageQueue = InMessageQueuePtr;

	const char* PipeNameA = TCHAR_TO_ANSI(*PipeName);
	
	FFastLogger::LogConsole(TEXT("Connecting To Pipe : %s"), *PipeName);
	Pipe = CreateFileA(
		PipeNameA,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
		);

	// TODO : 실패에 대한 처리가 필요함
	// 만약 연결이 실패했다면
	if (Pipe == INVALID_HANDLE_VALUE) {
		FFastLogger::LogConsole(TEXT("Failed to connect to pipe: %s"), *PipeName);
		return;
	}

	FFastLogger::LogConsole(TEXT("Succeeded to connect to pipe: %s"), *PipeName);
}

bool FIPCHandler::SendGameMessage(const FMessageUnion& Message)
{
	DWORD BytesWritten;
	if (!WriteFile(Pipe, Message.RawData, Message.Header.PayloadSize, &BytesWritten, NULL))
	{
		return false;
	}
	if (BytesWritten != Message.Header.PayloadSize)
	{
		FFastLogger::LogConsole(TEXT("Failed to send message: %s"), *PipeName);
		return false;
	}
	return true;
}

bool FIPCHandler::ReceiveMessage()
{
	DWORD BytesRead;

	if (BOOL Result = ReadFile(Pipe, Buffer, BufferSize, &BytesRead, NULL))
	{
		// 1. 버퍼에 읽은 내용을 누적 저장 (문자열 아님!)
		if (CachedLength + BytesRead > MaxCacheSize)
		{
			FFastLogger::LogConsole(TEXT("Buffer overflow detected: %s"), *PipeName);
			return false;
		}
		
		FMemory::Memcpy(CachedBuffer + CachedLength, Buffer, BytesRead);
		CachedLength += BytesRead;

		// 2. 최소한 헤더는 있어야 함
		if (CachedLength < sizeof(FMessageHeader))
			return false;

		// 3. 헤더 파싱
		FMessageHeader Header;
		FMemory::Memcpy(&Header, CachedBuffer, sizeof(FMessageHeader));

		// 4. 전체 메시지 길이 계산
		size_t TotalMessageSize = Header.PayloadSize;

		// 5. 전체 메시지가 도착했는지 확인
		if (CachedLength < TotalMessageSize)
			return false;

		// 6. 메시지 내용 파싱
		FMessageUnion Message;
		FMemory::Memcpy(&Message, CachedBuffer, Header.PayloadSize);

		// 7. 처리된 메시지만큼 뒤로 밀기
		size_t RemainingLength = CachedLength - TotalMessageSize;
		if (RemainingLength > 0)
		{
			std::memmove(CachedBuffer, CachedBuffer + TotalMessageSize, RemainingLength);
		}
		CachedLength = RemainingLength;

		// FFastLogger::LogConsole(TEXT("Received ByteSize : %d"), (int32)BytesRead);
		// FFastLogger::LogConsole(TEXT("Size of EyeTrackingNotifyMessage : %d"), sizeof(FEyeTrackingNotifyMessage));
		// FFastLogger::LogConsole(TEXT("Received Message Type : %hu"), Message.Header.Type);
		// FFastLogger::LogConsole(TEXT("Received Message Size : %hu"), Message.Header.PayloadSize);
		// // FFastLogger::LogConsole(TEXT("Received Message SessionID : %s"), ANSI_TO_TCHAR((const char*)Message.Header.SessionID));
		// FFastLogger::LogConsole(TEXT("Received Message PlayerID : %d"), Message.Header.PlayerID);
		// FFastLogger::LogConsole(TEXT("Received Message Start : %d, End : %d"), Message.EyeTrackingNotifyMessage.Start, Message.EyeTrackingNotifyMessage.End);
		// FFastLogger::LogConsole(TEXT("MessageQueue Size : %d"), (int32)MessageQueue->size());
		
		// 8. 처리 완료
		if (MessageQueue && MessageQueue->find(Message.Header.Type) != MessageQueue->end())
		{
			FFastLogger::LogConsole(TEXT("Add Message to Queue"));
			MessageQueue->at(Message.Header.Type).push(Message);
		}
		else
		{
			FFastLogger::LogConsole(TEXT("Message type not found in queue: %s"), *PipeName);
			return false;
		}
		
		return true;
	}
	else
	{
		// FFastLogger::LogConsole(TEXT("Failed to read message: %s"), *PipeName);
		return false;
	}
}

FIPCHandler::~FIPCHandler()
{
	CloseHandle(Pipe);
}
