// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PlayerID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerName;

	// 해당 플레이어가 준비 상태인지 여부 (예: 캐릭터 선택완료)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsReady;

	// 현재 생존 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAlive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsWin;

	// 기본 생성자
	FPlayerInfo()
		: PlayerID(-1)
		, PlayerName(TEXT("Unknown"))
		, bIsReady(false)
		, bIsAlive(true)
		, bIsWin(false)
	{}
};

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MapID;

	// 맵 썸네일?!
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Thumbnail;

	// 맵마다 맵을 설명하는 UI를 띄우자
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* LoadingWidget;
	
	// 기본 생성자
	FMapInfo()
		: MapID(-1)
		, Thumbnail(nullptr)
		, LoadingWidget(nullptr)
	{}
};