// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkPlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ALobbyPlayerController : public ANetworkPlayerController
{
	GENERATED_BODY()

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
};
