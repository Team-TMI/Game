// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingUI.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UGameSettingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 전환
	Btn_GameSet->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickGameSet);
	Btn_OtherSet->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickOtherSet);

	// 전체 음소거
	Btn_SoundOn->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickSoundOn);
	Btn_SoundOff->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickSoundOff);

	// 각각 사운드
	Sd_BGM->OnValueChanged.AddDynamic(this, &UGameSettingUI::OnBGMValueChanged);
	Sd_Weather->OnValueChanged.AddDynamic(this, &UGameSettingUI::OnWeatherValueChanged);
	Sd_UI->OnValueChanged.AddDynamic(this, &UGameSettingUI::OnUIValueChanged);
	Sd_Character->OnValueChanged.AddDynamic(this, &UGameSettingUI::OnCharacterValueChanged);
	Sd_Obstacle->OnValueChanged.AddDynamic(this, &UGameSettingUI::OnObstacleValueChanged);

	// 이펙트
	Btn_EffectOn->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickEffectOn);
	Btn_EffectOff->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickEffectOff);

	// 환경효과
	Btn_WeatherOn->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickWeatherOn);
	Btn_WeatherOff->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickWeatherOff);

	// 화면 밝기
	Sd_Light->OnValueChanged.AddDynamic(this, &UGameSettingUI::OnLightValueChanged);

	// 안티앨리어싱
	Btn_AntiOn->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickAntiOn);
	Btn_AntiOff->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickAntiOff);

	// 색약 설정
	ComboBox_Color->OnSelectionChanged.AddDynamic(this, &UGameSettingUI::OnColorModeChanged);

	// 뒤로가기
	Btn_QuitGame->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickQuitGame);
	Btn_GoBack->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickGoBack);
}

void UGameSettingUI::PlaySettingAnim(bool bIsForward)
{
	if (bIsForward)
	{
		PlayAnimationForward(SettingAnim);
	}
	else
	{
		PlayAnimationReverse(SettingAnim);
	}
}

void UGameSettingUI::OnClickGameSet()
{
	SettingSwitcher->SetActiveWidgetIndex(0);
}

void UGameSettingUI::OnClickOtherSet()
{
	SettingSwitcher->SetActiveWidgetIndex(1);
}

void UGameSettingUI::OnClickSoundOn()
{
}

void UGameSettingUI::OnClickSoundOff()
{
}

void UGameSettingUI::OnBGMValueChanged(float Value)
{
	/*float Volume = FMath::Clamp(Value, 0.0f, 100.0f);

	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		MasterSoundMix, // SoundMix* 변수로 선언하고 세팅
		BGMSoundClass,  // USoundClass* 변수로 선언하고 세팅
		Volume,
		1.0f,  // Pitch
		0.0f   // FadeInTime
	);

	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);*/
}

void UGameSettingUI::OnWeatherValueChanged(float Value)
{
}

void UGameSettingUI::OnUIValueChanged(float Value)
{
}

void UGameSettingUI::OnCharacterValueChanged(float Value)
{
}

void UGameSettingUI::OnObstacleValueChanged(float Value)
{
}

void UGameSettingUI::OnClickEffectOn()
{
}

void UGameSettingUI::OnClickEffectOff()
{
}

void UGameSettingUI::OnClickWeatherOn()
{
}

void UGameSettingUI::OnClickWeatherOff()
{
}

void UGameSettingUI::OnLightValueChanged(float Value)
{
}

void UGameSettingUI::OnClickAntiOn()
{
}

void UGameSettingUI::OnClickAntiOff()
{
}

void UGameSettingUI::OnColorModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "보정 없음")
	{
		// 색약 보정 끄기
	}
	else if (SelectedItem == "적색맹")
	{
		// Protanopia
	}
	else if (SelectedItem == "녹색맹")
	{
		// Deuteranopia
	}
	else if (SelectedItem == "청색맹")
	{
		// Tritanopia
	}
	else if (SelectedItem == "전색맹")
	{
		// Achromatopsia (전색맹)
	}
}

void UGameSettingUI::OnClickQuitGame()
{
}

void UGameSettingUI::OnClickGoBack()
{
	PlaySettingAnim(false);
}
