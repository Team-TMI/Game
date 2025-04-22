// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeTrackingProp.h"


#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "JumpGame/AIServices/Shared/IOManagerComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/UI/Obstacle/EyeTrackingUI.h"
#include "JumpGame/UI/Obstacle/FlyingObjectUI.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
AEyeTrackingProp::AEyeTrackingProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEyeTrackingProp::BeginPlay()
{
	Super::BeginPlay();

	FlyingObjectUI = CreateWidget<UFlyingObjectUI>(GetWorld(), FlyingObjectUIClass);
	if (FlyingObjectUI)
	{
		FlyingObjectUI->AddToViewport();
	}

	if (GEngine && GEngine->GameViewport)
	{
		// 내 뷰포트 사이즈 가져옴
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		FVector2D ScreenPosition(ScreenX, ScreenY);
		TrackingUI->SetPositionInViewport(ScreenPosition);
	}
}

void AEyeTrackingProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		SendEyeTrackingStart();
	}
}

void AEyeTrackingProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		SendEyeTrackingEnd();
	}
}

void AEyeTrackingProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
}

// Called every frame
void AEyeTrackingProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		SendEyeTrackingStart();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
		SendEyeTrackingEnd();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
	{
		RecvEyeTrackingInfo();
	}
}

void AEyeTrackingProp::SendEyeTrackingStart()
{
	FLog::Log("SendEyeTrackingStart");

	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	FMessageUnion SendMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.Type = EMessageType::EyeTrackingNotify;
	SendMessage.EyeTrackingNotifyMessage.Header.PayloadSize = sizeof(FEyeTrackingNotifyMessage);
	SendMessage.EyeTrackingNotifyMessage.Header.SessionID[0] = 1;
	SendMessage.EyeTrackingNotifyMessage.Header.PlayerID = 1;

	SendMessage.EyeTrackingNotifyMessage.QuizID = 100;
	SendMessage.EyeTrackingNotifyMessage.Start = 1;
	SendMessage.EyeTrackingNotifyMessage.End = 0;

	GS->IOManagerComponent->SendGameMessage(SendMessage);
}

void AEyeTrackingProp::SendEyeTrackingEnd()
{
	FLog::Log("SendEyeTrackingEnd");

	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	FMessageUnion SendMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.Type = EMessageType::EyeTrackingNotify;
	SendMessage.EyeTrackingNotifyMessage.Header.PayloadSize = sizeof(FEyeTrackingNotifyMessage);
	SendMessage.EyeTrackingNotifyMessage.Header.SessionID[0] = 1;
	SendMessage.EyeTrackingNotifyMessage.Header.PlayerID = 1;

	SendMessage.EyeTrackingNotifyMessage.QuizID = 100;
	SendMessage.EyeTrackingNotifyMessage.Start = 0;
	SendMessage.EyeTrackingNotifyMessage.End = 1;

	GS->IOManagerComponent->SendGameMessage(SendMessage);
}

void AEyeTrackingProp::RecvEyeTrackingInfo()
{
	FLog::Log("RecvEyeTrackingInfo");

	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	if (!GS)
	{
		FLog::Log("No GameState");
		return;
	}

	FMessageUnion RecvMessage;
	if (GS->IOManagerComponent->PopMessage(EMessageType::EyeTrackingResponse, RecvMessage))
	{
		QuizID = RecvMessage.EyeTrackingResponseMessage.QuizID;
		Width = RecvMessage.EyeTrackingResponseMessage.Width;
		Height = RecvMessage.EyeTrackingResponseMessage.Height;
		X = RecvMessage.EyeTrackingResponseMessage.X;
		Y = RecvMessage.EyeTrackingResponseMessage.Y;
		bBlink = RecvMessage.EyeTrackingResponseMessage.bBlink;
		State = RecvMessage.EyeTrackingResponseMessage.State;

		UE_LOG(LogTemp, Warning, TEXT("State: %d"), State);
		UE_LOG(LogTemp, Warning, TEXT("Blink: %d"), bBlink);
		UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f"), X, Y);
		UE_LOG(LogTemp, Warning, TEXT("Width: %f, Height: %f"), Width, Height);
		UE_LOG(LogTemp, Warning, TEXT("QuizID: %d"), QuizID);

		TrackLocation(FVector2f(Width, Height), FVector2f(X, Y));
	}
}

void AEyeTrackingProp::TrackLocation(FVector2f Resolution, FVector2f ScreenLoc)
{
	if (GEngine && GEngine->GameViewport)
	{
		// 내 뷰포트 사이즈 가져옴
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		// 정규화
		float NormalizedX{ScreenLoc.X / Resolution.X};
		float NormalizedY{ScreenLoc.Y / Resolution.Y};

		// Screen 좌표로 변환
		float ScreenX{static_cast<float>(NormalizedX * ViewportSize.X)};
		float ScreenY{static_cast<float>(NormalizedY * ViewportSize.Y)};

		// UI 인스턴스 없으면 생성
		if (!TrackingUI)
		{
			TrackingUI = CreateWidget<UEyeTrackingUI>(GetWorld(), EyeTrackingUIClass);
		}
		
		if (TrackingUI)
		{
			// 뷰포트에 없으면 추가
			if (!TrackingUI->IsInViewport())
			{
				TrackingUI->AddToViewport();
			}

			// 위치 갱신
			FVector2D ScreenPosition(ScreenX, ScreenY);
			TrackingUI->SetPositionInViewport(ScreenPosition);
		}
	}
}
