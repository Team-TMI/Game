// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <queue>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MessageHandlers/MessageHandlerInterface.h"
#include "MessageHandlers/PingMessageHandler.h"
#include "IOManageComponent.generated.h"


// GameState
// 클라든, 서버든 둘 다 통신을 해야하기 때문에
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UIOManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIOManageComponent();

	bool SendMessage(const FMessagePackage& MessagePackage);
	bool PopMessage(const EMessageType& MessageType, FMessageUnion& OutMessage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void RegisterMessageHandler(const EMessageType& MessageType, TSharedPtr<IMessageHandlerInterface> Handler);
	void RegisterIOHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// 어떤 식으로 데이터를 가공해서 보냄/받음
	TMap<EMessageType, TSharedPtr<IMessageHandlerInterface>> MessageHandlers;
	// 어떤 통신 기법으로 보내겠다. (Socket, IPC 등)
	TMap<EMessageType, TSharedPtr<IIOHandlerInterface>> IOHandlers;

	
	std::map<EMessageType, std::queue<FMessageUnion>> MessageQueue;
};
