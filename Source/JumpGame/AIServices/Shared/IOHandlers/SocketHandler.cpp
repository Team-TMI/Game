// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketHandler.h"

#include "WebSocketsModule.h"

FSocketHandler::FSocketHandler()
{
}

void FSocketHandler::Init()
{
	IIOHandlerInterface::Init();

	// ...
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	TWeakObjectPtr<FSocketHandler> WeakThis = this;
	
	Socket->OnConnected().AddLambda([WeakThis]() {
		if (!WeakThis.IsValid()) return;

		FSocketHandler* StrongThis = WeakThis.Get();
		
		UE_LOG(LogTemp, Warning, TEXT("Connected to WebSocket server."));
	});

	Socket->OnConnectionError().AddLambda([WeakThis](const FString& Error) {
		if (!WeakThis.IsValid()) return;

		FSocketHandler* StrongThis = WeakThis.Get();
		
		UE_LOG(LogTemp, Error, TEXT("Connection error: %s"), *Error);
	});

	Socket->OnRawMessage().AddLambda([WeakThis](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) {
		if (!WeakThis.IsValid()) return;

		FSocketHandler* StrongThis = WeakThis.Get();
	});

	Socket->Connect();
}

void FSocketHandler::SendMessage(const FMessageUnion& Message)
{
	Socket->Send(Message.RawData, Message.Header.PayloadSize, true);
}

void FSocketHandler::ReceiveMessage()
{
}

FSocketHandler::~FSocketHandler()
{
}
