// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpGaugeUI.h"

#include "Components/RadialSlider.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"

void UJumpGaugeUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Frog = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (Frog)
	{
		Frog->OnSuperJumpRatioChanged.AddDynamic(this, &UJumpGaugeUI::OnSuperJumpRatioChanged);
	}

	JumpGaugeSlider->IsFocusable = false;
	JumpGaugeSlider->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UJumpGaugeUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UJumpGaugeUI::OnSuperJumpRatioChanged(float NewRatio)
{
	JumpGaugeSlider->SetValue(NewRatio);

	if (NewRatio >= 1.f)
	{
		JumpGaugeSlider->SetSliderProgressColor(FColor::Red);
	}
	else if (NewRatio >= 0.5f)
	{
		JumpGaugeSlider->SetSliderProgressColor(FColor::Yellow);
	}
	else
	{
		JumpGaugeSlider->SetSliderProgressColor(FColor::Green);
	}
}
