// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeRemainUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UTimeRemainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void ChangeGaugeValue(float Value);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URadialSlider* MissionTimeRemainSlider;
};
