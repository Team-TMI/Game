// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IOHandlerInterface.h"
#include "IWebSocket.h"

/**
 * 
 */
class JUMPGAME_API FSocketHandler : public IIOHandlerInterface, public TSharedFromThis<FSocketHandler>
{
public:
	FSocketHandler();

	virtual void Init(const FIOHandlerInitInfo& InitInfo) override;
	virtual bool SendGameMessage(const FMessageUnion& Message) override;
	virtual bool ReceiveMessage() override { return true; };
	
	virtual ~FSocketHandler() override;
	
private:
	void ReceiveSocketMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	
	TSharedPtr<IWebSocket> Socket = nullptr;

	FString ServerURL;
	FString ServerProtocol;
};
