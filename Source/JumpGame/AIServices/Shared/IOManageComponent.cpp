#include "IOManageComponent.h"

#include "IOHandlers/IPCHandler.h"
#include "IOHandlers/SocketHandler.h"

UIOManageComponent::UIOManageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UIOManageComponent::SendMessage(const FMessagePackage& MessagePackage)
{
	if (!IOHandlers.Contains(MessagePackage.MessageType))
	{
		return false;
	}

	EMessageType MessageHandler = MessagePackage.MessageType;
	
	if (MessageHandlers.Contains(MessagePackage.MessageType))
	{
		if (MessageHandlers[MessageHandler]->SendMessage(MessagePackage, IOHandlers[MessageHandler]))
		{
			return true;
		}
		return false;
	}
	return false;
}

void UIOManageComponent::BeginPlay()
{
	Super::BeginPlay();

	// Message Hander 초기화
	RegisterMessageHandler(EMessageType::Ping, MakeShared<FPingMessageHandler>());
	RegisterMessageHandler(EMessageType::Ping, MakeShared<IMessageHandlerInterface>());
	RegisterMessageHandler(EMessageType::Pong, MakeShared<IMessageHandlerInterface>());
	RegisterMessageHandler(EMessageType::QuizNotify, MakeShared<IMessageHandlerInterface>());
	RegisterMessageHandler(EMessageType::WaveRequest, MakeShared<IMessageHandlerInterface>());
	RegisterMessageHandler(EMessageType::WaveResponse, MakeShared<IMessageHandlerInterface>());
	RegisterMessageHandler(EMessageType::EyeTrackingNotify, MakeShared<IMessageHandlerInterface>());
	RegisterMessageHandler(EMessageType::EyeTrackingResponse, MakeShared<IMessageHandlerInterface>());
	RegisterMessageHandler(EMessageType::EyeTrackingRequest, MakeShared<IMessageHandlerInterface>());

	// IOHandler 초기화
	TSharedPtr<FIPCHandler> IPCHandler = MakeShared<FIPCHandler>();
	TSharedPtr<FSocketHandler> SocketHandler = MakeShared<FSocketHandler>();

	RegisterIOHandler(EMessageType::Ping, SocketHandler);
	RegisterIOHandler(EMessageType::Pong, SocketHandler);
	RegisterIOHandler(EMessageType::QuizNotify, SocketHandler);
	RegisterIOHandler(EMessageType::WaveRequest, SocketHandler);
	RegisterIOHandler(EMessageType::WaveResponse, SocketHandler);

	RegisterIOHandler(EMessageType::EyeTrackingNotify, IPCHandler);
	RegisterIOHandler(EMessageType::EyeTrackingResponse, IPCHandler);
	RegisterIOHandler(EMessageType::EyeTrackingRequest, IPCHandler);
}

void UIOManageComponent::RegisterMessageHandler(const EMessageType& MessageType, TSharedPtr<IMessageHandlerInterface> Handler)
{
	MessageHandlers.Add(MessageType, Handler);
}

void UIOManageComponent::RegisterIOHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler)
{
	IOHandlers.Add(MessageType, Handler);
}

void UIOManageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UIOManageComponent::PopMessage(const EMessageType& MessageType, FMessageUnion& OutMessage)
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
