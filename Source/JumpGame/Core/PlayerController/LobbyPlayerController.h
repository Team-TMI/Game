// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkPlayerController.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "LobbyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextChatTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendListReceived, const TArray<FSteamFriendData>&, FriendList);

/**
 * 
 */
UCLASS()
class JUMPGAME_API ALobbyPlayerController : public ANetworkPlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController();
	
public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* NextChatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputMappingContext* LobbyMappingContext;

	// NextChatAction 시 호출될 함수
	void NextChat();

	UPROPERTY(BlueprintAssignable)
	FOnNextChatTriggered OnNextChatTriggered;
public:
	
	/*
	 * Return : 0 : 실패, 1 : 성공
	 * OutPitch : Pitch 비율 값
	 * OutRoll : Roll 비율 값
	 */
	UFUNCTION()
	int32 GetPitchYawRatio(float& OutPitch, float& OutRoll);

	UFUNCTION()
	float GetPitchRatio(FVector InLocallyPosition, FVector InLocallyMousePosition);
	UFUNCTION()
	float GetYawRatio(FVector InLocallyPosition, FVector InLocallyMousePosition);

private:
	UFUNCTION()
	void CalculateMinMax(const FVector& InLocallyPosition);

	// Min : X 좌표, Max : Y 좌표
	UPROPERTY()
	FVector2D MinusPitchMinMax = FVector2D(0.f, 0.f);
	UPROPERTY()
	FVector2D MinusYawMinMax = FVector2D(0.f, 0.f);
	UPROPERTY()
	FVector2D PlusPitchMinMax = FVector2D(0.f, 0.f);
	UPROPERTY()
	FVector2D PlusYawMinMax = FVector2D(0.f, 0.f);

public:
	// 팝업으로 띄울 UI
	UPROPERTY(editanywhere)
	TSubclassOf<class UBottomNaviBarUI> BottomNaviBarUIClass;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	UBottomNaviBarUI* BottomNaviBarUI;

public:
	UFUNCTION(Server, Reliable)
	void Server_RequestFriendList();

	UFUNCTION(Client, Reliable)
	void Client_ReceiveFriendList(const TArray<FSteamFriendData>& FriendList);

	UPROPERTY(BlueprintAssignable)
	FOnFriendListReceived OnFriendListUpdated;

private:
	TArray<FSteamFriendData> CollectedFriendList;
	
};
