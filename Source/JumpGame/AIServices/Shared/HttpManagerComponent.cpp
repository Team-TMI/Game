#include "HttpManagerComponent.h"

#include "IOHandlers/HttpsMultiPartsHandler.h"
#include "JumpGame/Utils/FastLogger.h"

UHttpManagerComponent::UHttpManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHttpManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO : HttpManagerComponent를 두자
	TSharedPtr<IIOHandlerInterface> HttpMultipartHandler = MakeShared<FHttpsMultiPartsHandler>();
	
	RegisterHttpHandler(EMessageType::HttpMultipartRequest, HttpMultipartHandler);

	for (auto& Handler : HttpHandlers)
	{
		HttpMessageQueue[Handler.Key] = std::queue<FHttpMessageWrapper>();
	}
	
	HttpMultipartHandler->Init(FIOHandlerInitInfo(), nullptr, &HttpMessageQueue);
}


bool UHttpManagerComponent::SendHttpMessage(const FHttpMessageWrapper& HttpMessage)
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

void UHttpManagerComponent::RegisterHttpHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler)
{
	FFastLogger::LogConsole(TEXT("UIOManagerComponent::RegisterHttpHandler : %d"), MessageType);
	HttpHandlers.Add(MessageType, Handler);
}

bool UHttpManagerComponent::PopHttpMessage(const EMessageType& MessageType, FHttpMessageWrapper& OutMessage)
{
	if (HttpMessageQueue.find(MessageType) != HttpMessageQueue.end())
	{
		if (HttpMessageQueue[MessageType].empty())
		{
			return false;
		}
		FHttpMessageWrapper Message = HttpMessageQueue[MessageType].front();
		HttpMessageQueue[MessageType].pop();
		OutMessage = Message;
		return true;
	}
	return false;
}
