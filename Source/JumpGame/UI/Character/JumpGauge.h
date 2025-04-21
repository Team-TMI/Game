// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGauge.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UJumpGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	class AFrog* Frog;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URadialSlider* JumpGaugeSlider;
};
