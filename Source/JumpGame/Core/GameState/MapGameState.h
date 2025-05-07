// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "MapGameState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AMapGameState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
