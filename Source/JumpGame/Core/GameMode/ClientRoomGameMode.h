// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ClientRoomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AClientRoomGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	// 업데이트 될 UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UClientRoomLogoUI> ClientRoomLogoUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClientRoomLogoUI* ClientRoomLogoUI;
};
