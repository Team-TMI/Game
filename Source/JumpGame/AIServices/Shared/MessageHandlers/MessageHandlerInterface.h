#pragma once

#include "MessagePackage.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "JumpGame/AIServices/Shared/IOHandlers/IOHandlerInterface.h"

class JUMPGAME_API IMessageHandlerInterface
{
public:
	IMessageHandlerInterface() {};

	// Send Message To Handler
	virtual bool SendMessage(const FMessagePackage& MessagePackage, const TSharedPtr<IIOHandlerInterface>& IOHandler) = 0;
	virtual void ReadMessage() = 0;
	
	virtual	~IMessageHandlerInterface() {};
};
