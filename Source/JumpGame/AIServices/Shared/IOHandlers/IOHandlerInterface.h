#pragma once

#include "CoreMinimal.h"

// Socket, IPC에 따라서 분기를 나누는 역할
class JUMPGAME_API IIOHandlerInterface
{
public:
	IIOHandlerInterface() {};

	virtual void Init() = 0;
	virtual void SendMessage(const FMessageUnion& Message) = 0;
	virtual void ReceiveMessage() = 0;
	
	virtual ~IIOHandlerInterface() {};
};
