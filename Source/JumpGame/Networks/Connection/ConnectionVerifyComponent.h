// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConnectionVerifyComponent.generated.h"

// Connection이 연결되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionSucceeded);
// Connection이 차단되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionBlocked);
// 서버 기준 Connection이 추가 되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientAdded, const FString&, NetID);
// 서버 기준 모든 클라이언트가 연결되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllClientAdded);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UConnectionVerifyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConnectionVerifyComponent();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// 서버가 총 플레이어 수를 설정한다.
	// 이게 설정이 되어야 타이머가 작동한다.
	void InitMaxPlayerCount(const int32 InMaxPlayerCount);

	UPROPERTY()
	FOnConnectionSucceeded OnConnectionSucceeded;
	UPROPERTY()
	FOnConnectionBlocked OnConnectionBlocked;
	UPROPERTY()
	FOnClientAdded OnClientAdded;
	UPROPERTY()
	FOnAllClientAdded OnAllClientAdded;
	
private:
	UFUNCTION()
	void RequestHandshake_Recursive();
	
	// Three Way Handshake를 통해 연결 상태를 확인한다.
	// 서버가 HandShake를 요청한다.
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RequestHandshake();
	// 클라이언트가 이에 응답한다.
	UFUNCTION(Server, Reliable)
	void ServerRPC_ConfirmHandshake(const FString& NetID, bool bConnSucceded);
	// 서버가 클라이언트에게 결과를 전달한다.
	UFUNCTION(Client, Reliable)
	void ClientRPC_NotifyConfirmHandshake(bool bConnSucceded);
	
	// 서버에서 연결된 클라이언트들의 상태를 확인하기 위한 함수
	UPROPERTY()
	TMap<FString, bool> ConnectionMap;

	// 클라이언트에서 Connection이 성공했는지 여부
	UPROPERTY()
	bool bConnectionSucceeded = false;
	
	UPROPERTY()
	float InitialConnectionInterval = 0.5f;
	UPROPERTY()
	FTimerHandle ConnectionTimerHandle;

	UPROPERTY()
	int32 MaxPlayerCount = 0;
};
