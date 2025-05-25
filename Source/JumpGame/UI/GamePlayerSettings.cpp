// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGamePlayerSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	// 1. 밝기/대비 적용 (어떻게 처리하는지에 따라 다를 수 있음)
	// 예: PostProcessMaterial에 파라미터로 넘긴다면 여기선 안 해도 됨

	// 2. 사운드 볼륨 적용
	USoundMix* MasterSoundMix = LoadObject<USoundMix>(nullptr, TEXT("/Game/Sounds/Class/SCM_TotalSound.SCM_TotalSound"));
	const float VolumeClamp = 1.0f;
	
	// 람다
	auto ApplyVolume = [this, MasterSoundMix](const FString& ClassName, float Volume)
	{
		USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *ClassName);
		if (SoundClass && MasterSoundMix)
		{
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, SoundClass, Volume, 1.0f, 0.1f);
		}
	};

	float MasterVolumeFloat = (MasterVolume == 1) ? 1.0f : 0.0f;
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Master.SC_Master"), MasterVolumeFloat);
	
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Background.SC_Background"), FMath::Clamp(BgmVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Nature.SC_Nature"), FMath::Clamp(WeatherVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_UI.SC_UI"), FMath::Clamp(UIVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_InGameEffect.SC_InGameEffect"), FMath::Clamp(CharacterVolume, 0.f, VolumeClamp));
	ApplyVolume(TEXT("/Game/Sounds/Class/SC_Obstacle.SC_Obstacle"), FMath::Clamp(ObstacleVolume, 0.f, VolumeClamp));

	// 3. 안티앨리어싱 적용
	GEngine->Exec(GetWorld(), *FString::Printf(TEXT("r.AntiAliasingMethod %d"), AntiValue));

	// 4. 색각 모드 적용
	switch (static_cast<EColorVisionDeficiency>(ColorMode))
	{
	case EColorVisionDeficiency::NormalVision:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::NormalVision, 1.0f, true, false);
		break;
	case EColorVisionDeficiency::Protanope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Protanope, 1.0f, true, false);
		break;
	case EColorVisionDeficiency::Deuteranope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Deuteranope, 1.0f, true, false);
		break;
	case EColorVisionDeficiency::Tritanope:
		UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(EColorVisionDeficiency::Tritanope, 1.0f, true, false);
		break;
	}
}

