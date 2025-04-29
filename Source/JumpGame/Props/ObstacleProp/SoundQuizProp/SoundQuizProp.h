// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoiceRecorderComponent.h"
#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Props/ObstacleProp/ObstacleProp.h"
#include "SoundQuizProp.generated.h"

class ARisingWaterProp;

UCLASS()
class JUMPGAME_API ASoundQuizProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundQuizProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class ANetworkGameState* NetGS;
	ARisingWaterProp* RisingWaterProp;
	// 퀴즈 실패했나요?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsQuizFail = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION()
	virtual void SendStartSoundQuizNotify();
	UFUNCTION()
	virtual void SendSoundQuizMessage();
	UFUNCTION()
	virtual void ReceiveSoundQuizMessage();
	UFUNCTION()
	virtual void SendEndSoundQuizNotify();

	UFUNCTION()
	virtual void ResetSoundQuiz();

	UFUNCTION()
	virtual void StartRecord();
	UFUNCTION()
	virtual void StopRecord();

	UPROPERTY()
	int32 QuizID = -1;
	UPROPERTY()
	float Similarity = 0.f;
	UPROPERTY()
	uint32 MessageSize = 0;
	UPROPERTY()
	FString MessageStr = "";

	// 음성데이터를 몇번 주고 받으면 정답을 맞추지 못해도 게임을 종료
	UPROPERTY()
	uint32 SendResponseIdx = 0;

public:
	// WAV 파일 로드 후 바이너리 데이터로 전환
	UPROPERTY()
	TArray<uint8> CachedBinaryWav;
	UPROPERTY()
	int32 CurrentSendIndex = 0;
	UPROPERTY()
	int32 TotalWavSize = 0;
	UPROPERTY()
	int32 ChunkSize = 1024;

	// UNUSED: 나중에 0.01초 재귀로 바뀔수도있음
	// FTimerHandle SendWavTimerHandle;
	
	UFUNCTION()
	void LoadWavFileBinary(const FString& FilePath, TArray<uint8>& BinaryData);

	UPROPERTY(EditAnywhere)
	class AFrog* Frog;

protected:
	// 녹음 컴포넌트 붙이기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UVoiceRecorderComponent> VoiceRecorderComponent = nullptr;

	UPROPERTY()
	FTimerHandle RecordStartTimer;
	UPROPERTY()
	FTimerHandle RecordStopTimer;

	// Tick을 활성화, 비활성화하는 변수
	UPROPERTY()
	bool bIsMessageReceived = false;
};
