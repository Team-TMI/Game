#pragma once

#include <map>
#include <queue>

#include "CoreMinimal.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "IOHandlerInitInfo.h"

// Socket, IPC에 따라서 분기를 나누는 역할
class JUMPGAME_API IIOHandlerInterface
{
public:
	IIOHandlerInterface() {};

	virtual bool Init(const FIOHandlerInitInfo& InitInfo, std::map<EMessageType, std::queue<FMessageUnion>>* InMessageQueuePtr) = 0;
	virtual bool SendGameMessage(const FMessageUnion& Message) = 0;
	virtual bool ReceiveMessage() = 0;
	virtual void SetReaderMode() {};
	virtual void SetWriterMode() {};
	
	virtual ~IIOHandlerInterface() {};

	bool bConnected = false;
protected:
	std::map<EMessageType, std::queue<FMessageUnion>>* MessageQueue = nullptr;
};
