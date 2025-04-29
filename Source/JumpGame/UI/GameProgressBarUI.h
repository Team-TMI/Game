// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "GameProgressBarUI.generated.h"

/**
 * 게임 진행도를 나타내는 프로그레스바
 * 추가사항: 각 플레이어 이동 정보, 물의 위치값
 */
UCLASS()
class JUMPGAME_API UGameProgressBarUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry & MyGeometry, float DeltaSeconds) override;

	UPROPERTY()
	class UJumpGameInstance* GI;
	UPROPERTY()
	TMap<FString, FPlayerInfo> PlayerInfo;
	
	UPROPERTY()
	class AFrog* Character;
	UPROPERTY()
	uint32 MyPlayerID = -1;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_Player;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* GameProgressBar;
	
	UPROPERTY()
	float GameProgress = 0;
	UPROPERTY()
	float StartPropZ = 0;
	UPROPERTY()
	float FinPropZ = 0;
	UPROPERTY()
	float WaterPropZ = 0;

	// 나를 포함한 모든 플레이어의 위치
	UPROPERTY()
	TArray<float> PlayerPos = { 0 };
	// 1등 플레이어의 위치
	UPROPERTY()
	float WinnerPos = 0;
	UPROPERTY()
	float TotalGamePos = 0;

	UFUNCTION()
	void UpdatePlayerPos();
	UFUNCTION()
	void UpdateProgressBar();

	UFUNCTION()
	void CreatePlayerMarkers();
	UFUNCTION()
	void UpdatePlayerMarkers();

	// 플레이어 목록 위젯
	UPROPERTY(editanywhere)
	TSubclassOf<class UPlayerMarkerWidget> PlayerMarkerFactory;
	UPROPERTY(editanywhere)
	UPlayerMarkerWidget* PlayerMarker;
	// 플레이어 목록 배열
	UPROPERTY(editanywhere)
	TArray<UPlayerMarkerWidget*> PlayerMarkers;
};
