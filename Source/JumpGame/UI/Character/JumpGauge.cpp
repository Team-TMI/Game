// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpGauge.h"

#include "Components/RadialSlider.h"
#include "JumpGame/Characters/Frog.h"

void UJumpGauge::NativeConstruct()
{
	Super::NativeConstruct();

	Frog = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UJumpGauge::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	JumpGaugeSlider->SetValue(Frog->SuperJumpRatio);

	if (JumpGaugeSlider->GetValue() >= 1.f)
	{
		JumpGaugeSlider->SetSliderProgressColor(FColor::Red);
	}
	else if (JumpGaugeSlider->GetValue() >= 0.5f)
	{
		JumpGaugeSlider->SetSliderProgressColor(FColor::Yellow);
	}
	else
	{
		JumpGaugeSlider->SetSliderProgressColor(FColor::Green);
	}
}
