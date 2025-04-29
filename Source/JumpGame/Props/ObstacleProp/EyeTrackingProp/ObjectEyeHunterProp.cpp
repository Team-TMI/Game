// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectEyeHunterProp.h"

#include "Components/Image.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/UI/Obstacle/EyeTrackingUI.h"
#include "JumpGame/UI/Obstacle/FlyingObjectUI.h"
#include "JumpGame/UI/Obstacle/TimeRemainUI.h"

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

	ConstructorHelpers::FClassFinder<UTimeRemainUI> TimeRemainWidget
		(TEXT("/Game/UI/Obstacle/WBP_TimeRemain.WBP_TimeRemain_C"));
	if (TimeRemainWidget.Succeeded())
	{
		TimeRemainUIClass = TimeRemainWidget.Class;
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

	InitializeMission();
}


void AObjectEyeHunterProp::InitializeMission()
{
	// 위젯 미리 생성
	FlyingObjectUI = CreateWidget<UFlyingObjectUI>(GetWorld(), FlyingObjectUIClass);
	TrackingUI = CreateWidget<UEyeTrackingUI>(GetWorld(), EyeTrackingUIClass);
	TimeRemainUI = CreateWidget<UTimeRemainUI>(GetWorld(), TimeRemainUIClass);

	if (TimeRemainUI)
	{
		// 미션 타이머 종료 바인드
		TimeRemainUI->OnMissionTimerEnd.AddDynamic(this, &AObjectEyeHunterProp::MissionTimeEnd);
	}

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
}


void AObjectEyeHunterProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                            AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                            bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                        SweepResult);

	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog = Cast<AFrog>(OtherActor);

		StartMission();
	}
}

void AObjectEyeHunterProp::StartMission()
{
	Super::SendEyeTrackingStart();

	StopCharacter();

	if (!FlyingObjectUI)
	{
		FlyingObjectUI = CreateWidget<UFlyingObjectUI>(GetWorld(), FlyingObjectUIClass);
	}

	FlyingObjectUI->InitializeParameters();

	if (!TimeRemainUI)
	{
		TimeRemainUI = CreateWidget<UTimeRemainUI>(GetWorld(), TimeRemainUIClass);
		TimeRemainUI->AddToViewport();
	}
	else
	{
		TimeRemainUI->AddToViewport();
	}

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Object가 움직일 경로 (비율로 저장)
	// 오른쪽 상단
	TargetPositionsRatio.Add(FVector2D(0.15f, 0.15f));
	// 오른쪽 하단
	TargetPositionsRatio.Add(FVector2D(0.15f, 0.85f));
	// 중간
	TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));
	// 왼쪽 상단
	TargetPositionsRatio.Add(FVector2D(0.85f, 0.15f));
	// 왼쪽 하단
	TargetPositionsRatio.Add(FVector2D(0.85f, 0.85f));
	// 중간
	TargetPositionsRatio.Add(FVector2D(0.5f, 0.5f));

	// 초기 목표 위치 계산
	SetTargetPositionsByViewport();

	CurrentTargetIndex = 0;
	BezierAlpha = 0.f;
	ObjectSpeed = 0.5f;

	if (FlyingObjectUI)
	{
		FlyingObjectUI->AddToViewport();

		// 내 Screen 좌표로 변환
		const float ScreenX{static_cast<float>(ViewportSize.X / 2)};
		const float ScreenY{static_cast<float>(ViewportSize.Y / 2)};

		ObjectScreenLocation = {ScreenX, ScreenY};

		FlyingObjectUI->SetPositionInViewport(ObjectScreenLocation);
	}

	if (TrackingUI)
	{
		TrackingUI->AddToViewport();	
	}
	
	// 미션 시작
	bIsStartHunt = true;

	// 시작 위치 설정
	StartPosition = ObjectScreenLocation;
	if (TargetPositions.IsValidIndex(CurrentTargetIndex))
	{
		// 초기 목표 위치 설정
		TargetPosition = TargetPositions[CurrentTargetIndex];
		// 초기 제어점들 생성, 랜덤 반경 설정
		ControlPoint1 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.0f);
		ControlPoint2 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.0f);
	}

	// 10초 후 미션 종료 ( 타이머 설정 )
	TimeRemainUI->StartMissionTimer(100.f);
}

void AObjectEyeHunterProp::StopCharacter()
{
	if (Frog)
	{
		Frog->SetActorLocation(MissionLocation->GetComponentLocation());
		Frog->CameraMissionMode();
		Frog->StopMovementAndResetRotation();
		Frog->SetCrouchEnabled(false);
	}
}

void AObjectEyeHunterProp::ResumeCharacter()
{
	if (Frog)
	{
		Frog->ResumeMovement();
		Frog->CameraMovementMode();
		Frog->SetCrouchEnabled(true);
	}
}

void AObjectEyeHunterProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		if (bIsStartHunt)
		{
			EndMission(false);
		}
	}
}

void AObjectEyeHunterProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsStartHunt)
	{
		return;
	}

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

	// 현재 위치를 이전 위치로 (회전 계산 위해)
	PreviousPosition = ObjectScreenLocation;
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

	// Viewport 크기 게임 중 바뀔 수 있으니까
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D CurrentViewportSize;
		GEngine->GameViewport->GetViewportSize(CurrentViewportSize);

		if (CurrentViewportSize != ViewportSize)
		{
			SetTargetPositionsByViewport();
			ViewportSize = CurrentViewportSize;
		}
	}

	RecvEyeTrackingInfo();
}

void AObjectEyeHunterProp::RecvEyeTrackingInfo()
{
	Super::RecvEyeTrackingInfo();

	// TODO : 받아오는 값으로 수정
	TrackLocation(static_cast<FVector2f>(ViewportSize), FVector2f(X, Y));
	//TrackLocation(FVector2f(Width, Height), FVector2f(X, Y));
}

void AObjectEyeHunterProp::FlyingObjectMovement(float DeltaTime)
{
	if (FlyingObjectUI)
	{
		if (!FlyingObjectUI->IsInViewport())
		{
			FlyingObjectUI->AddToViewport();
		}

		if (TargetPositions.IsValidIndex(CurrentTargetIndex))
		{
			BezierAlpha += ObjectSpeed * DeltaTime;

			// 도착하면
			if (BezierAlpha >= 1.f)
			{
				// 다시 초기화
				BezierAlpha = 0.f;
				// 목표 위치에 도달
				ObjectScreenLocation = TargetPosition;
				// 시작점 설정
				StartPosition = ObjectScreenLocation;
				SetNextTargetPosition();

				// 새로운 제어점 생성
				ControlPoint1 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.f);
				ControlPoint2 = GenerateRandomControlPoint(StartPosition, TargetPosition, 300.f);
			}
			else
			{
				// 베지어 곡선 따라 현재 위치 계산
				ObjectScreenLocation = GetBezierPoint(StartPosition, ControlPoint1, ControlPoint2,
				                                      TargetPosition,
				                                      BezierAlpha);
			}

			FlyingObjectUI->SetPositionInViewport(ObjectScreenLocation);
			UpdateObjectRotation(DeltaTime);
		}

		// 겹쳐있으면 Value 바꾸기
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
	// 정규화
	float NormalizedX{ScreenLoc.X / Resolution.X};
	float NormalizedY{ScreenLoc.Y / Resolution.Y};

	// 내 Screen 좌표로 변환
	float ScreenX{static_cast<float>(NormalizedX * ViewportSize.X)};
	float ScreenY{static_cast<float>(NormalizedY * ViewportSize.Y)};

	//FLog::Log("Loc", ScreenX, ScreenY);
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

bool AObjectEyeHunterProp::IsObjectAndEyeOverlap(FVector2D ObjectLocation, FVector2D EyeLocation)
{
	if (TrackingUI && FlyingObjectUI)
	{
		float Length{
			static_cast<float>(FMath::Abs(FVector2D::Distance(EyeLocation, ObjectLocation)))
		};

		if (Length <= 100.f)
		{
			return true;
		}
	}

	return false;
}

void AObjectEyeHunterProp::ChangeValue(bool bIsOverlap)
{
	if (bIsOverlap)
	{
		FlowTime = FMath::Clamp(FlowTime + (GetWorld()->GetDeltaSeconds()), 0.f, SuccessTime);
	}
	else
	{
		FlowTime = FMath::Clamp(FlowTime - (GetWorld()->GetDeltaSeconds() / 2), 0.f, SuccessTime);
	}

	SuccessRatio = FMath::Clamp(FlowTime / SuccessTime, 0.f, 1.f);
	FlyingObjectUI->ChangeGaugeValue(SuccessRatio);

	if (FMath::IsNearlyEqual(FlyingObjectUI->GetGaugeValue(), 1.f))
	{
		EndMission(true);
	}
}

void AObjectEyeHunterProp::MissionTimeEnd()
{
	EndMission(false);
}

void AObjectEyeHunterProp::EndMission(bool bIsSuccess)
{
	if (bIsSuccess)
	{
		// 미션 성공 : 발사 !
		//FLog::Log("Success");

		if (Frog)
		{
			FVector Direction{Frog->GetActorForwardVector() + FVector::UpVector};
			float Force{2'000};

			Super::LaunchCharacter(Frog, Direction, Force);
		}

		FlyingObjectUI->SuccessMission();
	}
	else
	{
		// 미션 실패 및 비정상 종료
		// FLog::Log("Failure");

		if (Frog)
		{
			FVector Direction{-1 * Frog->GetActorForwardVector() + FVector::UpVector};
			float Force{300};

			Super::LaunchCharacter(Frog, Direction, Force);
		}

		FlyingObjectUI->FailMission();
	}

	ResetMission();
}

void AObjectEyeHunterProp::ResetMission()
{
	ResumeCharacter();

	// 미션 끝
	bIsStartHunt = false;

	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);

	FlowTime = 0.f;
	SuccessRatio = 0.f;

	bIsDebug = false;

	ObjectScreenLocation = FVector2D::Zero();
	EyeScreenLocation = FVector2D::Zero();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	Super::SendEyeTrackingEnd();

	Frog = nullptr;

	if (TrackingUI)
	{
		TrackingUI->RemoveFromParent();
	}

	if (TimeRemainUI)
	{
		TimeRemainUI->RemoveFromParent();
	}
	
	//GetWorldTimerManager().ClearTimer(MissionTimerHandle);	
	GetWorldTimerManager().SetTimer(EndTimerHandle, FlyingObjectUI, &UFlyingObjectUI::VanishMission,
	                                0.5f, false);
}

void AObjectEyeHunterProp::SetNextTargetPosition()
{
	// 끝 도달하면 다시 0으로
	++CurrentTargetIndex;
	if (CurrentTargetIndex >= TargetPositions.Num())
	{
		CurrentTargetIndex = 0;
	}

	if (TargetPositions.IsValidIndex(CurrentTargetIndex))
	{
		TargetPosition = TargetPositions[CurrentTargetIndex];
	}
}

void AObjectEyeHunterProp::UpdateObjectRotation(float DeltaTime)
{
	if (FlyingObjectUI)
	{
		FVector2D Direction{(ObjectScreenLocation - PreviousPosition).GetSafeNormal()};

		// 방향 벡터가 (0, 0)이 아닌 경우에만 회전 계산
		if (Direction.SizeSquared() > 0.f)
		{
			// Radian 계산
			float AngleRad = FMath::Atan2(Direction.Y, Direction.X);
			// Degree로 변환
			float AngleDeg = FMath::RadiansToDegrees(AngleRad);

			if (UFlyingObjectUI* FlyUIWidget = Cast<UFlyingObjectUI>(FlyingObjectUI))
			{
				if (UImage* TargetImage = FlyUIWidget->TargetLocationImage)
				{
					// 이미지 회전
					TargetImage->SetRenderTransformAngle(AngleDeg + 180.f);
				}
			}
		}
	}
}

FVector2D AObjectEyeHunterProp::GetBezierPoint(FVector2D P0, FVector2D P1, FVector2D P2,
                                               FVector2D P3, float t)
{
	float u{1.f - t};
	float tt{t * t};
	float uu{u * u};
	float uuu{uu * u};
	float ttt{tt * t};

	// B(t) = P0(1 − t)^3 + 3 * P1 * t(1 - t)^2 + 3 * P2 * t^2(1 - t) + P3t^3

	// P0 * (1-t)^3
	FVector2D p{P0 * uuu};
	// 3 * P1 * (1-t)^2 * t
	p += 3.f * P1 * uu * t;
	// 3 * P2 * (1-t) * t^2
	p += 3.f * P2 * u * tt;
	// P3 * t^3
	p += P3 * ttt;

	return p;
}

FVector2D AObjectEyeHunterProp::GenerateRandomControlPoint(FVector2D StartPos, FVector2D EndPos,
                                                           float RandomRadius)
{
	FVector2D MidPoint{(StartPos + EndPos) / 2.0f};
	FVector2D RandomOffset{FMath::RandPointInCircle(RandomRadius)};

	// 시작, 끝 사이 무작위 지점 리턴
	return MidPoint + RandomOffset;
}

void AObjectEyeHunterProp::SetTargetPositionsByViewport()
{
	TargetPositions.Empty();
	for (const FVector2D& Ratio : TargetPositionsRatio)
	{
		TargetPositions.Add(FVector2D(ViewportSize.X * Ratio.X, ViewportSize.Y * Ratio.Y));
	}
}
