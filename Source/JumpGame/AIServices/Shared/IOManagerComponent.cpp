#include "IOManagerComponent.h"

#include "IOHandlers/HttpsMultiPartsHandler.h"
#include "IOHandlers/IPCHandler.h"
#include "IOHandlers/SocketHandler.h"
#include "JumpGame/Utils/FastLogger.h"

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

	FFastLogger::LogConsole(TEXT("UIOManagerComponent::SendGameMessage"));
	IOHandlers[MessageType]->SendGameMessage(Message);
	return true;
}

bool UIOManagerComponent::SendHttpMessage(const FHttpMessageWrapper& HttpMessage)
{
	EMessageType MessageType = HttpMessage.Header.Type;
	if (!HttpHandlers.Contains(MessageType))
	{
		return false;
	}
	
	FFastLogger::LogConsole(TEXT("UIOManagerComponent::SendHttpMessage"));
	HttpHandlers[MessageType]->SendGameMessage(HttpMessage);
	return true;
}

void UIOManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: 방어코드 재설정
	if (!GetWorld()->GetMapName().Contains(TEXT("InGameLevel")))
	{
		return ;
	}
	
	// IOHandler 초기화
	TSharedPtr<IIOHandlerInterface> IPCReadHandler = MakeShared<FIPCHandler>();
	IPCReadHandler->SetReaderMode();
	TSharedPtr<IIOHandlerInterface> IPCSendHandler = MakeShared<FIPCHandler>();
	IPCSendHandler->SetWriterMode();
	TSharedPtr<IIOHandlerInterface> SocketHandler = MakeShared<FSocketHandler>();
	
	RegisterIOHandler(EMessageType::Ping, SocketHandler);
	RegisterIOHandler(EMessageType::Pong, SocketHandler);
	RegisterIOHandler(EMessageType::QuizNotify, SocketHandler);
	RegisterIOHandler(EMessageType::WaveRequest, SocketHandler);
	RegisterIOHandler(EMessageType::WaveResponse, SocketHandler);
	
	RegisterIOHandler(EMessageType::EyeTrackingNotifyMessage, IPCSendHandler);
	RegisterIOHandler(EMessageType::EyeTrackingResponseMessage, IPCReadHandler);
	RegisterIOHandler(EMessageType::EyeTrackingRequestMessage, IPCReadHandler);

	TSharedPtr<IIOHandlerInterface> HttpMultipartHandler = MakeShared<FHttpsMultiPartsHandler>();
	
	RegisterHttpHandler(EMessageType::HttpMultipartRequest, HttpMultipartHandler);
	
	// queue 초기화
	for (auto& Handler : IOHandlers)
	{
		MessageQueue[Handler.Key] = std::queue<FMessageUnion>();
	}
	for (auto& Handler : HttpHandlers)
	{
		HttpMessageQueue[Handler.Key] = std::queue<FHttpMessageWrapper>();
	}

	if (!IPCReadHandler->Init(IOHandlerInitInfo, &MessageQueue, &HttpMessageQueue))
	{
		TSharedPtr<FIPCHandler> SharedIPC = StaticCastSharedPtr<FIPCHandler>(IPCReadHandler);
		RetryReadConnectToPipe(SharedIPC);
	}

	if (!IPCSendHandler->Init(IOHandlerInitInfo, &MessageQueue, &HttpMessageQueue))
	{
		TSharedPtr<FIPCHandler> SharedIPC = StaticCastSharedPtr<FIPCHandler>(IPCSendHandler);
		RetrySendConnectToPipe(SharedIPC);
	}

	SocketHandler->Init(IOHandlerInitInfo, &MessageQueue, &HttpMessageQueue);
	HttpMultipartHandler->Init(IOHandlerInitInfo, &MessageQueue, &HttpMessageQueue);
}

void UIOManagerComponent::RegisterIOHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler)
{
	IOHandlers.Add(MessageType, Handler);
}

void UIOManagerComponent::RegisterHttpHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler)
{
	HttpHandlers.Add(MessageType, Handler);
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

void UIOManagerComponent::RetryReadConnectToPipe(TSharedPtr<FIPCHandler> IPCHandlerToRetry)
{
	TWeakObjectPtr<UIOManagerComponent> WeakThis = this;

	GetWorld()->GetTimerManager().SetTimer(RetryReadTimer, FTimerDelegate::CreateLambda([WeakThis, IPCHandlerToRetry]()
	{
		if (!WeakThis.IsValid())
			return;

		UIOManagerComponent* StrongThis = WeakThis.Get();
		if (IPCHandlerToRetry->Init(StrongThis->IOHandlerInitInfo, &StrongThis->MessageQueue, &StrongThis->HttpMessageQueue))
		{
			FFastLogger::LogConsole(TEXT("PIPE!!!!!!!!!!! Succedded!! IPCHandler : %s"), *IPCHandlerToRetry->GetPipeName());
		}
		else
		{
			//FFastLogger::LogConsole(TEXT("Reconnecting to pipe... : %s"), *IPCHandlerToRetry->GetPipeName());
			StrongThis->RetryReadConnectToPipe(IPCHandlerToRetry);
		}
	}), RetryInterval, false);
}

void UIOManagerComponent::RetrySendConnectToPipe(TSharedPtr<FIPCHandler> IPCHandlerToRetry)
{
	TWeakObjectPtr<UIOManagerComponent> WeakThis = this;

	GetWorld()->GetTimerManager().SetTimer(RetrySendTimer, FTimerDelegate::CreateLambda([WeakThis, IPCHandlerToRetry]()
	{
		if (!WeakThis.IsValid())
			return;

		UIOManagerComponent* StrongThis = WeakThis.Get();
		if (IPCHandlerToRetry->Init(StrongThis->IOHandlerInitInfo, &StrongThis->MessageQueue, &StrongThis->HttpMessageQueue))
		{
			FFastLogger::LogConsole(TEXT("PIPE!!!!!!!!!!! Succedded!! IPCHandler : %s"), *IPCHandlerToRetry->GetPipeName());
		}
		else
		{
			//FFastLogger::LogConsole(TEXT("Reconnecting to pipe... : %s"), *IPCHandlerToRetry->GetPipeName());
			StrongThis->RetrySendConnectToPipe(IPCHandlerToRetry);
		}
	}), RetryInterval, false);
}
