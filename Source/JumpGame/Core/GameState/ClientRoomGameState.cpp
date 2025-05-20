// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomGameState.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "JumpGame/UI/Lobby/Follower.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AClientRoomGameState::AClientRoomGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	PrevLoc = FVector2D::ZeroVector;
}

void AClientRoomGameState::BeginPlay()
{
	Super::BeginPlay();

	FollowerUI = CreateWidget<UFollower>(GetWorld(), FollowerUIClass);
	if (FollowerUI)
	{
		FollowerUI->AddToViewport();
		APlayerController* PC{UGameplayStatics::GetPlayerController(this, 0)};
		PrevLoc = UWidgetLayoutLibrary::GetMousePositionOnViewport(PC);
	}
}

void AClientRoomGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PC{UGameplayStatics::GetPlayerController(this, 0)};
	FVector2D MousePos{};
	PC->GetMousePosition(MousePos.X, MousePos.Y);
	MousePos -= {-10.f, 25.f};

	if (FollowerUI)
	{
		FollowerUI->SetPositionInViewport(
			UKismetMathLibrary::Vector2DInterpTo(PrevLoc, MousePos, DeltaSeconds / 10.f, 10.f));
		float CurrentAngle{FollowerUI->GetRenderTransformAngle()};
		float TargetAngle{
			static_cast<float>(UKismetMathLibrary::DegAtan2((PrevLoc - MousePos).Y, (PrevLoc - MousePos).X))
		};
		TargetAngle = UKismetMathLibrary::FClamp(TargetAngle, -80.f, 80.f);
		FollowerUI->SetRenderTransformAngle(
			UKismetMathLibrary::FInterpTo(CurrentAngle, TargetAngle, DeltaSeconds / 10.f, 10.f));
	}

	PrevLoc = MousePos;
}
