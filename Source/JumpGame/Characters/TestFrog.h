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

	UFUNCTION()
	void OnPressCKey();
	UFUNCTION()
	void OnPressEmotionKey(int32 EmotionIndex);
	UFUNCTION()
	void ShowEmotionUI(bool bIsShow);
	UFUNCTION()
	void CancleEmotion();
	UFUNCTION()
	void PlayAnimation(int32 EmotionIndex);

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEmotionUI> EmotionUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEmotionUI* EmotionUI;

	// 재생할 몽타주
	UPROPERTY()
	UAnimMontage* CurrentEmotionMontage = nullptr;
};
