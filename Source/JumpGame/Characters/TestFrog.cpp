// Fill out your copyright notice in the Description page of Project Settings.


#include "TestFrog.h"

#include "Blueprint/UserWidget.h"
#include "JumpGame/UI/Character/EmotionUI.h"
#include "JumpGame/Utils/FastLogger.h"


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
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased(EKeys::C))
	{
		OnReleasedCKey();
	}
}

// Called to bind functionality to input
void ATestFrog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ATestFrog::OnPressCKey()
{	
	if (EmotionState == EEmotionState::None || EmotionState == EEmotionState::PlayingEmotion)
	{
		ShowEmotionUI(true);
		EmotionState = EEmotionState::WaitingForInput;
	}
}

void ATestFrog::OnReleasedCKey()
{
	if (EmotionState == EEmotionState::WaitingForInput)
	{
		EmotionUI->ConfirmEmotionSelection();
		EmotionState = EEmotionState::PlayingEmotion;
		ShowEmotionUI(false);
	}
}

void ATestFrog::OnSelectionEmotionIndex(int32 EmotionIndex)
{
	// 선택된 감정 처리
	ShowEmotionUI(false);
	ServerPlayEmotion(EmotionIndex);
	FFastLogger::LogScreen(FColor::Red, TEXT("선택인덱스: %d"), EmotionIndex);
}

void ATestFrog::ShowEmotionUI(bool bIsShow)
{
	if (!EmotionUI) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (bIsShow)
	{
		EmotionUI->PlayShowAnim(true);

		if (!bIsBind)
		{
			EmotionUI->OnEmotionSelected.BindUObject(this, &ATestFrog::OnSelectionEmotionIndex);
			bIsBind = true;
		}
		
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		EmotionUI->PlayShowAnim(false);

		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameAndUI());
	}
}

void ATestFrog::CancelEmotion()
{
	if (EmotionState == EEmotionState::PlayingEmotion && CurrentEmotionMontage)
	{
		StopAnimMontage(CurrentEmotionMontage);
		CurrentEmotionMontage = nullptr;
	}

	EmotionState = EEmotionState::None;
	
	// TODO: 이동할때 취소되는지 확인 필요
	// InputAxis MoveForward, InputAxis MoveRight 등에 직접 CancelEmotion 호출?
	// 그냥 input 들어오면 바꾸기?
}

void ATestFrog::PlayEmotion(int32 EmotionIndex)
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

void ATestFrog::ServerPlayEmotion_Implementation(int32 EmotionIndex)
{
	MulticastPlayEmotion(EmotionIndex);
}

void ATestFrog::MulticastPlayEmotion_Implementation(int32 EmotionIndex)
{
	PlayEmotion(EmotionIndex);
}
