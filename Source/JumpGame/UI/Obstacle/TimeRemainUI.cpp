// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeRemainUI.h"

#include "Components/RadialSlider.h"

void UTimeRemainUI::NativeConstruct()
{
	Super::NativeConstruct();

	ChangeGaugeValue(0.f);
}

void UTimeRemainUI::ChangeGaugeValue(float Value)
{
	MissionTimeRemainSlider->SetValue(Value);
}
