// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeTrackingProp.h"


#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/UI/Obstacle/EyeTrackingUI.h"
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

	QuizID = 100;
	Start = 1;
	End = 0;

	// FEyeTrackingRequest.QuizID = QuizID;
	// FEyeTrackingRequest.Start = Start;
	// FEyeTrackingRequest.End = End;

	// send
}

void AEyeTrackingProp::SendEyeTrackingEnd()
{
	FLog::Log("SendEyeTrackingEnd");

	QuizID = 100;
	Start = 0;
	End = 1;

	// FEyeTrackingRequest.QuizID = QuizID;
	// FEyeTrackingRequest.Start = Start;
	// FEyeTrackingRequest.End = End;

	// send
}

void AEyeTrackingProp::RecvEyeTrackingInfo()
{
	FLog::Log("RecvEyeTrackingInfo");

	// recv

	// QuizID = FEyeTrackingResponse.QuizID;
	// Start = FEyeTrackingResponse.Start;
	// End = FEyeTrackingResponse.End;
	// Width = FEyeTrackingResponse.Width;
	// Height = FEyeTrackingResponse.Height;
	// X = FEyeTrackingResponse.X;
	// Y = FEyeTrackingResponse.Y;
	// bBlink = FEyeTrackingResponse.bBlink;
	// State = FEyeTrackingResponse.State;

	QuizID = 123;
	Start = 1;
	End = 0;
	Width = 1000;
	Height = 1000;
	X = 500;
	Y = 500;
	bBlink = 0;
	State = 100;

	UE_LOG(LogTemp, Warning, TEXT("State: %d"), State);
	UE_LOG(LogTemp, Warning, TEXT("Blink: %d"), bBlink);
	UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f"), X, Y);
	UE_LOG(LogTemp, Warning, TEXT("Width: %f, Height: %f"), Width, Height);
	UE_LOG(LogTemp, Warning, TEXT("Start: %d, End: %d"), Start, End);
	UE_LOG(LogTemp, Warning, TEXT("QuizID: %d"), QuizID);

	TrackLocation(FVector2f(Width, Height), FVector2f(X, Y));
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
		
		UEyeTrackingUI* TrackingUI{CreateWidget<UEyeTrackingUI>(GetWorld(), EyeTrackingUIClass)};
		if (TrackingUI)
		{
			TrackingUI->AddToViewport();
			FVector2D ScreenPosition(ScreenX, ScreenY);
			TrackingUI->SetPositionInViewport(ScreenPosition);
		}
	}


}
