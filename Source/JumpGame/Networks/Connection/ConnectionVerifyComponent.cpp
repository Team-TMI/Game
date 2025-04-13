// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionVerifyComponent.h"

#include "GameFramework/PlayerState.h"
#include "JumpGame/Utils/FastLogger.h"

// Sets default values for this component's properties
UConnectionVerifyComponent::UConnectionVerifyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}

void UConnectionVerifyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Type::Destroyed)
	{
		GetWorld()->GetTimerManager().ClearTimer(ConnectionTimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}

// 서버가 총 플레이어 수를 설정한다.
void UConnectionVerifyComponent::InitMaxPlayerCount(const int32 InMaxPlayerCount)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	
	MaxPlayerCount = InMaxPlayerCount;
	if (MaxPlayerCount <= 0)
	{
		FFastLogger::LogConsole(TEXT("ConnectionVerifyComponent::InitMaxPlayerCount: MaxPlayerCount is less than 0"));
		return;
	}
	
	RequestHandshake_Recursive();
}

void UConnectionVerifyComponent::RequestHandshake_Recursive()
{
	GetWorld()->GetTimerManager().ClearTimer(ConnectionTimerHandle);
	
	// 전부 연결된 상태라면 더이상 요청하지 않는다.
	if (ConnectionMap.Num() >= MaxPlayerCount)
	{
		return;
	}
	
	// 서버에서 클라이언트에게 Handshake를 요청한다.
	MulticastRPC_RequestHandshake();

	GetWorld()->GetTimerManager().SetTimer(ConnectionTimerHandle, this, &ThisClass::RequestHandshake_Recursive, InitialConnectionInterval, false);
}

// 클라이언트는 서버에게 Handshake에 대한 응답을 한다.
void UConnectionVerifyComponent::MulticastRPC_RequestHandshake_Implementation()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}
	APlayerState* PS = PC->GetPlayerState<APlayerState>();
	if (!PS)
	{
		return;
	}

	const FUniqueNetIdRepl& NetID = PS->GetUniqueId();
	const FUniqueNetIdPtr& NetIDPtr = NetID.GetUniqueNetId();
	if (!NetIDPtr.IsValid())
	{
		return;
	}
	const FString& NetIDString = NetIDPtr->ToString();
	
	// 서버에서 클라이언트에게 Handshake에 대한 응답을 한다.
	ServerRPC_ConfirmHandshake(NetIDString, true);
}

// 서버는 클라이언트에게 Handshake가 성공적으로 완료되었음을 알린다.
void UConnectionVerifyComponent::ServerRPC_ConfirmHandshake_Implementation(const FString& NetID, bool bConnSucceded)
{
	if (ConnectionMap.Contains(NetID))
	{
		return;
	}
	ConnectionMap.Add(NetID, bConnSucceded);
	OnClientAdded.Broadcast(NetID);
	if (ConnectionMap.Num() >= MaxPlayerCount)
	{
		OnAllClientAdded.Broadcast();
	}
	ClientRPC_NotifyConfirmHandshake(true);
}

// 클라이언트만 이 함수를 받으면 됨
void UConnectionVerifyComponent::ClientRPC_NotifyConfirmHandshake_Implementation(bool bConnSucceded)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		return;
	}
	
	bConnectionSucceeded = bConnSucceded;
	
	if (bConnectionSucceeded)
	{
		OnConnectionSucceeded.Broadcast();
	}
	else
	{
		OnConnectionBlocked.Broadcast();
	}
}
