// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NetworkGameState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ANetworkGameState : public AGameState
{
	GENERATED_BODY()

public:
	ANetworkGameState();

#pragma region NetworkClock

public:
	// 서버 시계를 가져온다.
	virtual double GetServerWorldTimeSeconds() const override;
	
private:
	// 서버의 시간 (앞으로 모든 클라이언트들이 해당 시간을 동기화 할 거임)
	UPROPERTY()
	double ServerWorldTimeSeconds = 0.0f;
	
public:
	
#pragma endregion
};
