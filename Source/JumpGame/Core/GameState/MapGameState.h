// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "MapGameState.generated.h"

// 모든 클라이언트가 연결되었을때 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllClientMapAdded);

UCLASS()
class JUMPGAME_API AMapGameState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	AMapGameState();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnAllClientAdded() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_AllClientAdded();

	UPROPERTY()
	FOnAllClientMapAdded OnAllClientAddedDelegate;

	// UI에 플레이어 위치를 동기화하자
	UPROPERTY()
	TArray<float> AllPlayerZPos;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateAllPlayerZPos(const TArray<float>& NewPlayerZPos);

	// 프로그레스바 UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameProgressBarUI> UGameProgressBarUIClass;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class UGameProgressBarUI* ProgressBarUI;
};
