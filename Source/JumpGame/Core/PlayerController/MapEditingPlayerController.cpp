// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditingPlayerController.h"

AMapEditingPlayerController::AMapEditingPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bShowMouseCursor = true;
	// Super::SetInputMode(FInputModeGameAndUI());
}

FVector AMapEditingPlayerController::GetMouseWorldPosition() const
{
	FVector MouseWorldPosition = FVector::ZeroVector;

	// FVector Start = GetPosition
	FVector2D MouseScreenPosition;
	bool bIsValid = GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y);

	// 마우스가 화면 밖에 있으면 무시
	if (!bIsValid) return FVector::ZeroVector;

	FHitResult HitResult;
	GetHitResultAtScreenPosition(MouseScreenPosition, ECC_Visibility, true, HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		MouseWorldPosition = HitResult.Location;
		DrawDebugSphere(GetWorld(), MouseWorldPosition, 10.f, 12, FColor::Red, false, 5.f);
	}
	else
	{
		// 마우스가 화면 밖에 있으면 무시
		return FVector::ZeroVector;
	}

	return MouseWorldPosition;
}
