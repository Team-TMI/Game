// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "TypeInfo/GameInfo.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ALobbyGameState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	ALobbyGameState();
	virtual void BeginPlay() override;
	virtual void OnClientAdded(const FString& NetID) override;

	UPROPERTY()
	class UJumpGameInstance* GI;
	
	// 플레이어가 들어올때마다 보내줄 내용 (로비 이미지 업데이트)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateWaitImage(const FString& PlayerKey, const FPlayerInfo& PlayerInfo);

	// 업데이트 될 UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UWaitRoomUI> WaitRoomUIClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UWaitRoomUI* WaitRoomUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ULoadMapComponent* LoadMapComponent;
};
