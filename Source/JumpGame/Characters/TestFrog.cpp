// Fill out your copyright notice in the Description page of Project Settings.


#include "TestFrog.h"

#include "Blueprint/UserWidget.h"
#include "JumpGame/UI/Character/EmotionUI.h"


// Sets default values
ATestFrog::ATestFrog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestFrog::BeginPlay()
{
	Super::BeginPlay();
	
	EmotionUI = CreateWidget<class UEmotionUI>(GetWorld(), EmotionUIClass);
	if (EmotionUI)
	{
		EmotionUI->AddToViewport();
	}
}

// Called every frame
void ATestFrog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::C))
	{
		OnPressCKey();
	}
}

// Called to bind functionality to input
void ATestFrog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ATestFrog::OnPressCKey()
{	
	if (EmotionState == EEmotionState::None)
	{
		ShowEmotionUI(true);
		EmotionState = EEmotionState::WaitingForInput;
	}
	else if (EmotionState == EEmotionState::WaitingForInput)
	{
		ShowEmotionUI(false);
		EmotionState = EEmotionState::None;
	}
}

void ATestFrog::OnPressEmotionKey(int32 EmotionIndex)
{
	if (EmotionState == EEmotionState::WaitingForInput)
	{
		PlayAnimation(EmotionIndex); 
		EmotionState = EEmotionState::PlayingEmotion;
		ShowEmotionUI(false);
	}
}

void ATestFrog::ShowEmotionUI(bool bIsShow)
{
	if (!EmotionUI) return;
		
	if (bIsShow)
	{
		EmotionUI->PlayShowAnim(true);
	}
	else
	{
		EmotionUI->PlayShowAnim(false);
	}
}

void ATestFrog::CancleEmotion()
{
	if (EmotionState == EEmotionState::PlayingEmotion && CurrentEmotionMontage)
	{
		StopAnimMontage(CurrentEmotionMontage);
		CurrentEmotionMontage = nullptr;
	}

	EmotionState = EEmotionState::None;
	
	// 이동할때 취소
	// InputAxis MoveForward, InputAxis MoveRight 등에 직접 CancelEmotion 호출?
	// 그냥 input 들어오면 바꾸기?
}

void ATestFrog::PlayAnimation(int32 EmotionIndex)
{
	// 카메라 제어 막기?
	
	/*switch (EmotionIndex)
	{
	case 1:
		CurrentEmotionMontage = GreetingMontage;
		break;
	case 2:
		CurrentEmotionMontage = AngryMontage;
		break;
	case 3:
		CurrentEmotionMontage = SadMontage;
		break;
	default:
		return;
	}

	PlayAnimMontage(CurrentEmotionMontage);
	EmotionState = EEmotionState::PlayingEmotion;*/
}
