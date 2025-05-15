// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Frog.h"
#include "TestFrog.generated.h"

UENUM()
enum class EEmotionState
{
	None,
	WaitingForInput,
	PlayingEmotion,
};

UCLASS()
class JUMPGAME_API ATestFrog : public AFrog
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestFrog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EEmotionState EmotionState = EEmotionState::None;

	// 감정표현 관련
	UPROPERTY()
	bool bIsBind = false;
	
	UFUNCTION()
	void OnPressCKey();
	UFUNCTION()
	void OnReleasedCKey();
	UFUNCTION()
	void OnSelectionEmotionIndex(int32 EmotionIndex);
	UFUNCTION()
	void CancelEmotion();
	UFUNCTION()
	void PlayEmotion(int32 EmotionIndex);

	// 동기화 (서버로 요청->처리)
	UFUNCTION(Server, Reliable)
	void ServerPlayEmotion(int32 EmotionIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayEmotion(int32 EmotionIndex);

	// 재생할 몽타주
	UPROPERTY()
	UAnimMontage* CurrentEmotionMontage = nullptr;

	// 감정표현 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEmotionUI> EmotionUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEmotionUI* EmotionUI;
	
	UFUNCTION()
	void ShowEmotionUI(bool bIsShow);
};


