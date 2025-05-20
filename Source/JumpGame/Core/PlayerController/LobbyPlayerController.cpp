// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "JumpGame/UI/BottomNaviBarUI.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFinder(
		TEXT("/Game/Characters/Input/LobbyInput/IA_NextChat.IA_NextChat"));
	if (InputActionFinder.Succeeded())
	{
		NextChatAction = InputActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContextFinder(
		TEXT("/Game/Characters/Input/LobbyInput/IMC_Lobby.IMC_Lobby"));
	if (InputContextFinder.Succeeded())
	{
		LobbyMappingContext = InputContextFinder.Object;
	}
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())})
	{
		Subsystem->AddMappingContext(LobbyMappingContext, 0);
	}

	BottomNaviBarUI = CreateWidget<UBottomNaviBarUI>(GetWorld(), BottomNaviBarUIClass);
	if (BottomNaviBarUI)
	{
		BottomNaviBarUI->AddToViewport(5);
	}
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(InputComponent)})
	{
		if (NextChatAction)
		{
			EnhancedInputComponent->BindAction(NextChatAction, ETriggerEvent::Started, this, &ALobbyPlayerController::NextChat);
		}
	}
}

void ALobbyPlayerController::NextChat()
{
	OnNextChatTriggered.Broadcast();
}

int32 ALobbyPlayerController::GetPitchYawRatio(float& OutPitch, float& OutYaw)
{
	// 현재 Viewtarget 중인 카메라를 가져온다.
	AActor* ViewTarget = GetViewTarget();

	// 카메라의 트랜스폼을 구한다.
	FTransform CameraTransform = ViewTarget->GetTransform();

	// Player의 좌표를 가져온다. (월드)
	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(),ALobbyFrog::StaticClass()));
	FVector PlayerLocation = Frog->GetActorLocation() + FVector(0, 0, 25);

	// Player의 좌표를 카메라의 로컬 좌표계로 변환한다.
	FVector InLocallyPosition = CameraTransform.InverseTransformPosition(PlayerLocation);

	// 마우스의 좌표를 가져온다.
	// 이게 근데 UI 위에 있으면 무시되려나.. 근데 그때는 뭐 상관없긴 함
	FVector2D MouseScreenPosition;
	if (!GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y))
	{
		return 0;
	}

	// 캐릭터의 위치와 ViewPort의 사이즈를 기반으로 MinusMinMax와 PlusMinMax를 구한다.
	CalculateMinMax(InLocallyPosition);

	// 마우스의 월드 좌표를 가져온다.
	FVector MouseWorldPosition;
	FVector MouseWorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, MouseScreenPosition, MouseWorldPosition, MouseWorldDirection))
	{
		return 0;
	}

	// 마우스의 좌표를 카메라의 로컬 좌표계로 변환한다.
	FVector InLocallyMousePosition = CameraTransform.InverseTransformPosition(MouseWorldPosition);

	// 두 개의 좌표로 Pitch와 Yaw의 비율을 구한다.
	OutPitch = GetPitchRatio(InLocallyPosition, InLocallyMousePosition);
	OutYaw = GetYawRatio(InLocallyPosition, InLocallyMousePosition);

	return 1;
}

float ALobbyPlayerController::GetPitchRatio(FVector InLocallyPosition, FVector InLocallyMousePosition)
{
	// InLocallyMousePosition - InLocallyPosition : 비율을 계산하기 위한 벡터
	FVector RatioVector = InLocallyMousePosition - InLocallyPosition;

	float PitchRatioFromVector = RatioVector.Z;
	float Sign = FMath::Sign(PitchRatioFromVector);

	float AbsRatio = 0;
	if (Sign > 0)
	{
		AbsRatio = FMath::Abs(PitchRatioFromVector / PlusPitchMinMax.Y);
	}
	else
	{
		AbsRatio = FMath::Abs(PitchRatioFromVector / MinusPitchMinMax.Y);
	}
	return Sign * AbsRatio;
}

float ALobbyPlayerController::GetYawRatio(FVector InLocallyPosition, FVector InLocallyMousePosition)
{
	// InLocallyMousePosition - InLocallyPosition : 비율을 계산하기 위한 벡터
	FVector RatioVector = InLocallyMousePosition - InLocallyPosition;

	float YawRatioFromVector = RatioVector.Y;
	float Sign = FMath::Sign(YawRatioFromVector);

	float AbsRatio = 0;
	if (Sign > 0)
	{
		AbsRatio = FMath::Abs(YawRatioFromVector / PlusYawMinMax.Y);
	}
	else
	{
		AbsRatio = FMath::Abs(YawRatioFromVector / MinusYawMinMax.Y);
	}
	return Sign * AbsRatio;
}

void ALobbyPlayerController::CalculateMinMax(const FVector& InLocallyPosition)
{
	// ViewPort의 사이즈를 구한다.
	FIntVector2 ViewportSize;
	GetViewportSize(ViewportSize.X, ViewportSize.Y);

	// 좌상단 좌표와 우하단 좌표를 구한다.
	FVector2D TopLeft = FVector2D(0.f, 0.f);
	FVector2D BottomRight = FVector2D(ViewportSize.X, ViewportSize.Y);

	// 위의 좌표를 기준으로 월드 좌표를 구한다.
	FVector TopLeftWorld;
	FVector TopLeftWorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, TopLeft, TopLeftWorld, TopLeftWorldDirection))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Deproject Top Left World ScreenToWorld failed"));
		return;
	}

	FVector BottomRightWorld;
	FVector BottomRightWorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, BottomRight, BottomRightWorld, BottomRightWorldDirection))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Deproject Bottom Right ScreenToWorld failed"));
		return;
	}

	// 월드 좌표를 카메라의 로컬 좌표계로 변환한다.
	FTransform CameraTransform = GetViewTarget()->GetTransform();
	FVector InLocallyTopLeft = CameraTransform.InverseTransformPosition(TopLeftWorld);
	FVector InLocallyBottomRight = CameraTransform.InverseTransformPosition(BottomRightWorld);

	// 위의 좌표로 Max와 Min을 각각 구한다.
	FVector InMinusMinMax = InLocallyTopLeft - InLocallyPosition;
	FVector InPlusMinMax = InLocallyBottomRight - InLocallyPosition;

	// InMinusMinMax로 Minus Pitch 과 Yaw을 구한다.
	// X : NOT USE | Y : Yaw | Z : Pitch
	MinusPitchMinMax.Y = InMinusMinMax.Z;
	MinusYawMinMax.Y = InMinusMinMax.Y;

	// InPlusMinMax로 Plus Pitch 과 Yaw을 구한다.
	// X : NOT USE | Y : Yaw | Z : Pitch
	PlusPitchMinMax.Y = InPlusMinMax.Z;
	PlusYawMinMax.Y = InPlusMinMax.Y;
}
