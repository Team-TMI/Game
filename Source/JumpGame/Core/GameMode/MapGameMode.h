// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameMode.h"
#include "MapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AMapGameMode : public ANetworkGameMode
{
	GENERATED_BODY()

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
};
