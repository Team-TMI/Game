// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingObjectUI.h"

#include "Components/Image.h"

void UFlyingObjectUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UFlyingObjectUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UFlyingObjectUI::ChangeGaugeValue(float Value)
{
	Value = FMath::Clamp(Value, 0.0f, 1.0f);
	
	GaugeValue = Value;
	
	GaugeImage->GetDynamicMaterial()->SetScalarParameterValue(FName(TEXT("Gauge")), GaugeValue);
}

float UFlyingObjectUI::GetGaugeValue()
{
	return GaugeValue;
}
