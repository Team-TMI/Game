// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IOHandlerInterface.h"
#include "IWebSocket.h"
#include "JumpGame/AIServices/Shared/Message.h"

/**
 * 
 */
class JUMPGAME_API FSocketHandler : public IIOHandlerInterface
{
public:
	FSocketHandler();

	virtual void Init() override;
	virtual void SendMessage(const FMessageUnion& Message) override;
	virtual void ReceiveMessage() override;
	
	virtual ~FSocketHandler();

	const FString ServerURL = TEXT("ws://192.168.20.68:8029/ws");
	const FString ServerProtocol = TEXT("ws");
	
private:
	TSharedPtr<IWebSocket> Socket = nullptr;
};
