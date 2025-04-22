#include "IOManagerComponent.h"

#include "IOHandlers/IPCHandler.h"
#include "IOHandlers/SocketHandler.h"

UIOManagerComponent::UIOManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UIOManagerComponent::SendGameMessage(const FMessageUnion& Message)
{
	EMessageType MessageType = Message.Header.Type;
	if (!IOHandlers.Contains(MessageType))
	{
		return false;
	}

	IOHandlers[MessageType]->SendGameMessage(Message);
	return true;
}

void UIOManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// IOHandler 초기화
	TSharedPtr<IIOHandlerInterface> IPCHandler = MakeShared<FIPCHandler>();
	TSharedPtr<IIOHandlerInterface> SocketHandler = MakeShared<FSocketHandler>();
	
	RegisterIOHandler(EMessageType::Ping, SocketHandler);
	RegisterIOHandler(EMessageType::Pong, SocketHandler);
	RegisterIOHandler(EMessageType::QuizNotify, SocketHandler);
	RegisterIOHandler(EMessageType::WaveRequest, SocketHandler);
	RegisterIOHandler(EMessageType::WaveResponse, SocketHandler);
	
	RegisterIOHandler(EMessageType::EyeTrackingNotify, IPCHandler);
	RegisterIOHandler(EMessageType::EyeTrackingResponse, IPCHandler);
	RegisterIOHandler(EMessageType::EyeTrackingRequest, IPCHandler);
	
	// queue 초기화
	for (auto& Handler : IOHandlers)
	{
		MessageQueue[Handler.Key] = std::queue<FMessageUnion>();
	}

	IPCHandler->Init(IOHandlerInitInfo, &MessageQueue);
	SocketHandler->Init(IOHandlerInitInfo, &MessageQueue);

	// // Dummy Message
	// for (int32 i = 0; i < 100; i++)
	// {
	// 	FPingMessage PingMessage;
	// 	PingMessage.Header.Type = EMessageType::Ping;
	// 	PingMessage.Header.PayloadSize = sizeof(FPingMessage);
	// 	PingMessage.Header.PlayerID = 0;
	// 	PingMessage.TimeStamp = FPlatformTime::Seconds();
	//
	// 	FMessageUnion MessageUnion;
	// 	FMemory::Memcpy(&MessageUnion, &PingMessage, sizeof(FPingMessage));
	// 	MessageQueue[EMessageType::Ping].push(MessageUnion);
	// }
}

void UIOManagerComponent::RegisterIOHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler)
{
	IOHandlers.Add(MessageType, Handler);
}

void UIOManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto& Handler : IOHandlers)
	{
		Handler.Value->ReceiveMessage();
	}
}

bool UIOManagerComponent::PopMessage(const EMessageType& MessageType, FMessageUnion& OutMessage)
{
	if (MessageQueue.find(MessageType) != MessageQueue.end())
	{
		if (MessageQueue[MessageType].empty())
		{
			return false;
		}
		FMessageUnion Message = MessageQueue[MessageType].front();
		MessageQueue[MessageType].pop();
		OutMessage = Message;
		return true;
	}
	return false;
}
