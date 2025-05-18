// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSettingUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UGameSettingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	// 나왔다 들어갔다 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SettingAnim;
	UFUNCTION()
	void PlaySettingAnim(bool bIsForward);
	
	// 전환
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SettingSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SettingPanel;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameSet;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_OtherSet;

	UFUNCTION()
	void OnClickGameSet();
	UFUNCTION()
	void OnClickOtherSet();

	// WidgetSwitcher(0)
	// 버튼별 기능_사운드
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SoundOn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SoundOff;

	UFUNCTION()
	void OnClickSoundOn();
	UFUNCTION()
	void OnClickSoundOff();

	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_BGM;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Weather;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_UI;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Character;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Obstacle;

	UFUNCTION()
	void OnBGMValueChanged(float Value);
	UFUNCTION()
	void OnWeatherValueChanged(float Value);
	UFUNCTION()
	void OnUIValueChanged(float Value);
	UFUNCTION()
	void OnCharacterValueChanged(float Value);
	UFUNCTION()
	void OnObstacleValueChanged(float Value);
	
	// 버튼별 기능_나머지
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_EffectOn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_EffectOff;

	UFUNCTION()
	void OnClickEffectOn();
	UFUNCTION()
	void OnClickEffectOff();
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_WeatherOn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_WeatherOff;

	UFUNCTION()
	void OnClickWeatherOn();
	UFUNCTION()
	void OnClickWeatherOff();

	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Light;
	
	UFUNCTION()
	void OnLightValueChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AntiOn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AntiOff;

	UFUNCTION()
	void OnClickAntiOn();
	UFUNCTION()
	void OnClickAntiOff();
	
	// WidgetSwitcher(1)
	// 색약설정
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox_Color;

	UFUNCTION()
	void OnColorModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// 뒤로가기
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_QuitGame;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoBack;

	UFUNCTION()
	void OnClickQuitGame();
	UFUNCTION()
	void OnClickGoBack();
};
