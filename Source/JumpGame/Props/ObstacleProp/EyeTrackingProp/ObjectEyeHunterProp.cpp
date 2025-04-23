// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectEyeHunterProp.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/UI/Obstacle/EyeTrackingUI.h"
#include "JumpGame/UI/Obstacle/FlyingObjectUI.h"

AObjectEyeHunterProp::AObjectEyeHunterProp()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UEyeTrackingUI> EyeTrackingUIWidget
		(TEXT("/Game/UI/Obstacle/WBP_EyeTracking.WBP_EyeTracking_C"));
	if (EyeTrackingUIWidget.Succeeded())
	{
		EyeTrackingUIClass = EyeTrackingUIWidget.Class;
	}

	ConstructorHelpers::FClassFinder<UFlyingObjectUI> FlyingObjectWidget
		(TEXT("/Game/UI/Obstacle/WBP_FlyingObject.WBP_FlyingObject_C"));
	if (FlyingObjectWidget.Succeeded())
	{
		FlyingObjectUIClass = FlyingObjectWidget.Class;
	}

	MissionLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MissionLocation"));
	if (MissionLocation)
	{
		MissionLocation->SetupAttachment(RootComponent);
		MissionLocation->SetRelativeLocation(FVector(0, 0, 100.f));
	}
}

void AObjectEyeHunterProp::BeginPlay()
{
	Super::BeginPlay();
}

void AObjectEyeHunterProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog = Cast<AFrog>(OtherActor);

		FlyingObjectUI = CreateWidget<UFlyingObjectUI>(GetWorld(), FlyingObjectUIClass);
		if (FlyingObjectUI)
		{
			FlyingObjectUI->AddToViewport();

			if (GEngine && GEngine->GameViewport && GEngine->GameUserSettings)
			{
				FVector2D ViewportSize;
				GEngine->GameViewport->GetViewportSize(ViewportSize);

				// 내 Screen 좌표로 변환
				const float ScreenX{static_cast<float>(ViewportSize.X / 2)};
				const float ScreenY{static_cast<float>(ViewportSize.Y / 2)};

				ObjectScreenLocation = {ScreenX, ScreenY};

				FlyingObjectUI->SetPositionInViewport(ObjectScreenLocation);

				StartMission();
			}
		}
	}
}

void AObjectEyeHunterProp::StartMission()
{
	Super::SendEyeTrackingStart();

	StopCharacter();

	// 미션 시작
	bIsStartHunt = true;
}

void AObjectEyeHunterProp::StopCharacter()
{
	if (Frog)
	{
		Frog->SetActorLocation(MissionLocation->GetComponentLocation());
		Frog->CameraMissionMode();
		Frog->StopMovementAndResetRotation();
	}
}

void AObjectEyeHunterProp::ResumeCharacter()
{
	if (Frog)
	{
		Frog->ResumeMovement();
		Frog->CameraMovementMode();
	}
}

void AObjectEyeHunterProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog = nullptr;

		if (bIsStartHunt)
		{
			EndMission(false);
		}
	}
}

void AObjectEyeHunterProp::Tick(float DeltaTime)
{
	if (!bIsStartHunt)
	{
		return;
	}

	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
	{
		if (!bIsDebug)
		{
			bIsDebug = true;
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
		else
		{
			bIsDebug = false;
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
	}

	FlyingObjectMovement(DeltaTime);

	if (bIsDebug && TrackingUI)
	{
		if (GEngine && GEngine->GameViewport)
		{
			FVector2D MousePosition;
			GEngine->GameViewport->GetMousePosition(MousePosition);

			EyeScreenLocation = {MousePosition.X, MousePosition.Y};

			TrackingUI->SetPositionInViewport(EyeScreenLocation);
		}
	}
}

void AObjectEyeHunterProp::RecvEyeTrackingInfo()
{
	Super::RecvEyeTrackingInfo();

	TrackLocation(FVector2f(Width, Height), FVector2f(X, Y));
}

void AObjectEyeHunterProp::FlyingObjectMovement(float DelaTime)
{
	if (FlyingObjectUI)
	{
		if (!FlyingObjectUI->IsInViewport())
		{
			FlyingObjectUI->AddToViewport();
		}

		// 날라가는 위치 경로
		float ScreenX{static_cast<float>(300)};
		float ScreenY{static_cast<float>(300)};

		ObjectScreenLocation = {ScreenX, ScreenY};

		FlyingObjectUI->SetPositionInViewport(ObjectScreenLocation);

		if (IsObjectAndEyeOverlap(ObjectScreenLocation, EyeScreenLocation))
		{
			ChangeValue(true);
		}
		else
		{
			ChangeValue(false);
		}
	}
}

void AObjectEyeHunterProp::TrackLocation(FVector2f Resolution, FVector2f ScreenLoc)
{
	if (GEngine && GEngine->GameViewport)
	{
		// 내 뷰포트 사이즈 가져옴
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		// 정규화
		float NormalizedX{ScreenLoc.X / Resolution.X};
		float NormalizedY{ScreenLoc.Y / Resolution.Y};

		// 내 Screen 좌표로 변환
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
			EyeScreenLocation = {ScreenX, ScreenY};
			TrackingUI->SetPositionInViewport(EyeScreenLocation);
		}
	}
}

bool AObjectEyeHunterProp::IsObjectAndEyeOverlap(FVector2D ObjectLocation, FVector2D EyeLocation)
{
	if (TrackingUI && FlyingObjectUI)
	{
		float Length{static_cast<float>(FMath::Abs(FVector2D::Distance(EyeLocation, ObjectLocation)))};

		if (Length <= 100.f)
		{
			ChangeValue(true);
			return true;
		}
	}

	ChangeValue(false);
	return false;
}

void AObjectEyeHunterProp::ChangeValue(bool bIsOverlap)
{
	if (bIsOverlap)
	{
		FlowTime = FMath::Clamp(FlowTime + (GetWorld()->GetDeltaSeconds() / 2), 0.f, SuccessTime);
	}
	else
	{
		FlowTime = FMath::Clamp(FlowTime - (GetWorld()->GetDeltaSeconds() / 3), 0.f, SuccessTime);
	}

	SuccessRatio = FMath::Clamp(FlowTime / SuccessTime, 0.f, 1.f);
	FlyingObjectUI->ChangeGaugeValue(SuccessRatio);

	if (FMath::IsNearlyEqual(FlyingObjectUI->GetGaugeValue(), 1.f))
	{
		EndMission(true);
	}
}


void AObjectEyeHunterProp::EndMission(bool bIsSuccess)
{
	if (bIsSuccess)
	{
		// 미션 성공 : 발사 !
		FLog::Log("Success");

		if (Frog)
		{
			FVector Direction{Frog->GetActorForwardVector() + FVector::UpVector};
			float Force{2'000};

			Super::LaunchCharacter(Frog, Direction, Force);
		}
	}
	else
	{
		// 미션 실패 및 비정상 종료
		FLog::Log("Failure");
	}

	ResetMission();
}


void AObjectEyeHunterProp::ResetMission()
{
	ResumeCharacter();

	// 미션 끝
	bIsStartHunt = false;

	FlowTime = 0.f;
	SuccessRatio = 0.f;

	bIsDebug = false;
	Frog = nullptr;

	ObjectScreenLocation = FVector2D::Zero();
	EyeScreenLocation = FVector2D::Zero();

	if (FlyingObjectUI)
	{
		FlyingObjectUI->RemoveFromParent();
	}

	if (TrackingUI)
	{
		TrackingUI->RemoveFromParent();
	}

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	Super::SendEyeTrackingEnd();
}
