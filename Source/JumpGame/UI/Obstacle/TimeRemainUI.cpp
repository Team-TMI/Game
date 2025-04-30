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

void UTimeRemainUI::StartMissionTimer(float InMissionTime)
{
	// 타이머를 원하는 시간
	MissionTime = InMissionTime;
	// 지금 얼마나 시간이 흘렀는지
	CurrentMissionTime = 0;

	// 미션 시작되면 시간을 줄이자
	GetWorld()->GetTimerManager().SetTimer(MissionTimerHandle, this,
		&UTimeRemainUI::MissionTimerTick, GetWorld()->GetDeltaSeconds(), true);
}

void UTimeRemainUI::StopMissionTimer()
{
	// 타이머 끝
	GetWorld()->GetTimerManager().ClearTimer(MissionTimerHandle);
}

void UTimeRemainUI::MissionTimerTick()
{
	CurrentMissionTime += GetWorld()->GetDeltaSeconds();

	ChangeGaugeValue(CurrentMissionTime / MissionTime);
	if (CurrentMissionTime >= MissionTime)
	{
		StopMissionTimer();
		// 미션 종료 알람을 보내자
		OnMissionTimerEnd.Broadcast();
	}
}
