// Fill out your copyright notice in the Description page of Project Settings.


#include "PingMessageHandler.h"


bool FPingMessageHandler::SendMessage(const FMessagePackage& MessagePackage,
	const TSharedPtr<IIOHandlerInterface>& IOHandler)
{
	FMessageUnion MessageUnion;

	MessageUnion.Header = MessagePackage.MessageUnion.Header;
	MessageUnion.PingMessage.TimeStamp = MessagePackage.MessageUnion.PingMessage.TimeStamp;

	IOHandler->SendMessage(MessageUnion);
	
	return true;
}
