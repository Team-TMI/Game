// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IOHandlerInterface.h"
#include "JumpGame/AIServices/Shared/Message.h"

/**
 * 
 */
class JUMPGAME_API FIPCHandler : public IIOHandlerInterface
{
public:
	FIPCHandler();

	virtual void SendMessage(const FMessageUnion& Message) override;
	virtual void ReceiveMessage() override;
	
	virtual ~FIPCHandler();
};
