// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageHandlerInterface.h"

/**
 * 
 */
class JUMPGAME_API FPingMessageHandler : public IMessageHandlerInterface
{
public:
	FPingMessageHandler();

	// Send Message To Handler
	virtual bool SendMessage(const FMessagePackage& MessagePackage, const TSharedPtr<IIOHandlerInterface>& IOHandler);
	virtual void ReadMessage() = 0;
	
	virtual ~FPingMessageHandler();

private:
};
