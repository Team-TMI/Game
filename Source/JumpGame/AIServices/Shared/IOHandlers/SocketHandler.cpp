// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketHandler.h"

#include "WebSocketsModule.h"

FSocketHandler::FSocketHandler()
{
}

void FSocketHandler::Init(const FIOHandlerInitInfo& InitInfo)
{
	ServerURL = InitInfo.ServerUrl;
	ServerProtocol = InitInfo.ServerProtocol;
	MessageQueue = InitInfo.MessageQueue;
	// ...
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	TWeakPtr<FSocketHandler> WeakSharedThis = AsShared();
	
	Socket->OnConnected().AddLambda([WeakSharedThis]() {
		if (!WeakSharedThis.IsValid()) return;

		TSharedPtr<FSocketHandler> StrongThis = WeakSharedThis.Pin();
		
		UE_LOG(LogTemp, Warning, TEXT("Connected to WebSocket server."));
	});

	Socket->OnConnectionError().AddLambda([WeakSharedThis](const FString& Error) {
		if (!WeakSharedThis.IsValid()) return;

		TSharedPtr<FSocketHandler> StrongThis = WeakSharedThis.Pin();
		
		UE_LOG(LogTemp, Error, TEXT("Connection error: %s"), *Error);
	});
	
	Socket->OnRawMessage().AddLambda([WeakSharedThis](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) {
		if (!WeakSharedThis.IsValid()) return;

		TSharedPtr<FSocketHandler> StrongThis = WeakSharedThis.Pin();

		StrongThis->ReceiveSocketMessage(Data, Size, BytesRemaining);
	});

	Socket->Connect();
}

bool FSocketHandler::SendGameMessage(const FMessageUnion& Message)
{
	Socket->Send(Message.RawData, Message.Header.PayloadSize, true);
	return true;
}

void FSocketHandler::ReceiveSocketMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	const uint8* RawData = (const uint8*)Data;

	FMessageUnion Message;
	// memory copy
	FMemory::Memcpy(&Message, RawData, Size);
	if (MessageQueue && MessageQueue->find(Message.Header.Type) != MessageQueue->end())
	{
		MessageQueue->at(Message.Header.Type).push(Message);
	}
}

FSocketHandler::~FSocketHandler()
{
	
}
