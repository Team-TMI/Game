// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingUI.h"

#include "GameQuitUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"
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
	Sd_Bgm->OnValueChanged.AddDynamic(this, &UGameSettingUI::OnBgmValueChanged);
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
	AntiAliasingMap.Add(TEXT("사용 안함"), 0);   // None
	AntiAliasingMap.Add(TEXT("FXAA"), 1);  // Fast Approximate
	AntiAliasingMap.Add(TEXT("TAA"), 2);   // Temporal AA
	// 콤보박스에 한글 표시 추가
	for (const auto& Pair : AntiAliasingMap)
	{
		ComboBox_Anti->AddOption(Pair.Key);
	}
	// 콜백 연결
	ComboBox_Anti->OnSelectionChanged.AddDynamic(this, &UGameSettingUI::OnAntiModeChanged);

	// 색약 설정
	// Enum → 표시 문자열 매핑
	ColorBlindMap.Add(TEXT("보정 없음"), EColorBlindMode::None);
	ColorBlindMap.Add(TEXT("적색약"),    EColorBlindMode::Protanope);
	ColorBlindMap.Add(TEXT("녹색약"),    EColorBlindMode::Deuteranope);
	ColorBlindMap.Add(TEXT("청색약"),    EColorBlindMode::Tritanope);
	// 콤보박스에 한글 표시 추가
	for (const auto& Pair : ColorBlindMap)
	{
		ComboBox_Color->AddOption(Pair.Key);
	}
	// 콜백 연결
	ComboBox_Color->OnSelectionChanged.AddDynamic(this, &UGameSettingUI::OnColorModeChanged);

	// 뒤로가기
	Btn_QuitGame->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickQuitGame);
	Btn_GoBack->OnClicked.AddDynamic(this, &UGameSettingUI::OnClickGoBack);

	GameQuitUI = CreateWidget<UGameQuitUI>(GetWorld(), GameQuitUIClass);
	if (GameQuitUI)
	{
		GameQuitUI->AddToViewport(15);
	}
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
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, SC_Master, 1.0f, 1.0f, 0.1f);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);

	Sd_Bgm->SetIsEnabled(true);
	Sd_Weather->SetIsEnabled(true);
	Sd_UI->SetIsEnabled(true);
	Sd_Character->SetIsEnabled(true);
	Sd_Obstacle->SetIsEnabled(true);
	
	Btn_SoundOn->SetIsEnabled(false);
	Btn_SoundOff->SetIsEnabled(true);
}

void UGameSettingUI::OnClickSoundOff()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, SC_Master, 0.0f, 1.0f, 0.1f);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);

	Sd_Bgm->SetIsEnabled(false);
	Sd_Weather->SetIsEnabled(false);
	Sd_UI->SetIsEnabled(false);
	Sd_Character->SetIsEnabled(false);
	Sd_Obstacle->SetIsEnabled(false);

	Btn_SoundOn->SetIsEnabled(true);
	Btn_SoundOff->SetIsEnabled(false);
}

void UGameSettingUI::OnBgmValueChanged(float Value)
{
	float Volume = FMath::Clamp(Value, 0.0f, 1.0f);

	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		MasterSoundMix,       // 대상 SoundMix
		SoundClass_Bgm,        // 변경할 SoundClass
		Volume,            // Volume
		1.0f,            // Pitch
		0.1f             // FadeInTime (몇 초에 걸쳐 적용될지)
	);

	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
}

void UGameSettingUI::OnWeatherValueChanged(float Value)
{
	float Volume = FMath::Clamp(Value, 0.0f, 1.0f);

	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		MasterSoundMix,
		SoundClass_Nature,
		Volume,
		1.0f,
		0.1f
	);

	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
}

void UGameSettingUI::OnUIValueChanged(float Value)
{
	float Volume = FMath::Clamp(Value, 0.0f, 1.0f);

	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		MasterSoundMix,
		SoundClass_UI,
		Volume,
		1.0f,
		0.1f
	);

	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
}

void UGameSettingUI::OnCharacterValueChanged(float Value)
{
	float Volume = FMath::Clamp(Value, 0.0f, 1.0f);

	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		MasterSoundMix,
		SoundClass_InGameEffect,
		Volume,
		1.0f,
		0.1f
	);

	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
}

void UGameSettingUI::OnObstacleValueChanged(float Value)
{
	float Volume = FMath::Clamp(Value, 0.0f, 1.0f);

	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		MasterSoundMix,
		SoundClass_Obstacle,
		Volume,
		1.0f,
		0.1f
	);

	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
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

void UGameSettingUI::SetAntiAliasingQuality(const FString& SelectedOption)
{
	// Mode: 0 = None, 1 = FXAA, 2 = TAA
	if (AntiAliasingMap.Contains(SelectedOption))
	{
		int32 Mode = AntiAliasingMap[SelectedOption];
		FString Cmd = FString::Printf(TEXT("r.AntiAliasingMethod %d"), Mode);
		if (GEngine)
		{
			GEngine->Exec(GetWorld(), *Cmd);
		}
	}
}

void UGameSettingUI::OnAntiModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!AntiAliasingMap.Contains(SelectedItem)) return;
	
	SetAntiAliasingQuality(SelectedItem);
}

void UGameSettingUI::OnColorModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!ColorBlindMap.Contains(SelectedItem)) return;

	EColorBlindMode SelectedMode = ColorBlindMap[SelectedItem];

	switch (SelectedMode)
	{
	case EColorBlindMode::None:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::NormalVision, 1.0f, true, false);
		break;
	case EColorBlindMode::Protanope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Protanope, 1.0f, true, false);
		break;
	case EColorBlindMode::Deuteranope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Deuteranope, 1.0f, true, false);
		break;
	case EColorBlindMode::Tritanope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Tritanope, 1.0f, true, false);
		break;
	}
}

void UGameSettingUI::OnClickQuitGame()
{
	if (GameQuitUI)
	{
		GameQuitUI->PlayQuitAnim(true);
	}
}

void UGameSettingUI::OnClickGoBack()
{
	PlaySettingAnim(false);
}
