// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkPlayerController.h"
#include "MapEditingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AMapEditingPlayerController : public ANetworkPlayerController
{
	GENERATED_BODY()

public:
	AMapEditingPlayerController();

	UFUNCTION()
	FVector GetMouseWorldPosition(FHitResult& HitResult) const;
};
