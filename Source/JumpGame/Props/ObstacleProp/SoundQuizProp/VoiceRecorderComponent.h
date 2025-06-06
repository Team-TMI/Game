// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioCaptureComponent.h"
#include "Sound/SampleBufferIO.h"
#include "VoiceRecorderComponent.generated.h"

// ========================================================================
// UVoiceRecorderComponent
// Synth component class which implements USynthComponent
// This is a simple hello-world type example which generates a sine-wave
// tone using a DSP oscillator class and implements a single function to set
// the frequency. To enable example:
// 1. Ensure "SignalProcessing" is added to project's .Build.cs in PrivateDependencyModuleNames
// 2. Enable macro below that includes code utilizing SignalProcessing Oscilator
// ========================================================================

// 음성 녹음을 시작/끝낼 때 호출할 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartRecording);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRecording);


#define SYNTHCOMPONENT_EX_OSCILATOR_ENABLED 0

UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class JUMPGAME_API UVoiceRecorderComponent : public UAudioCaptureComponent
{
	GENERATED_BODY()
	
	// Called when synth is created
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;

	// Sets the oscillator's frequency
	UFUNCTION(BlueprintCallable, Category = "Synth|Components|Audio")
	void SetFrequency(const float FrequencyHz = 440.0f);

protected:
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// A simple oscillator class. Can also generate Saw/Square/Tri/Noise.
	Audio::FOsc Osc;
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED

public:
	UFUNCTION(BlueprintCallable, Category = "RecordObstacle")
	void StartRecording();
	UFUNCTION(BlueprintCallable, Category = "RecordObstacle")
	void StopRecording(const bool bIsStop = false);
	UFUNCTION(BlueprintCallable, Category = "RecordObstacle")
	const bool IsRecording() const;

protected:
	FOnStartRecording OnStartRecording;
	FOnStopRecording OnStopRecording;
	
public:
	FOnStartRecording& GetOnStartRecording() { return OnStartRecording; }
	FOnStopRecording& GetOnStopRecording() { return OnStopRecording; }

protected:
	UPROPERTY(EditAnywhere, Category = "RecordObstacle")
	TObjectPtr<USoundSubmix> RecordSoundSubmix;

	TUniquePtr<Audio::FAudioRecordingData> RecordingData;
};