// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryPage.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UVictoryPage : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerName;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoLobby;

	void OnClickGoLobby();
	void SetVictoryPlayerName(FString PlayerName);
};
