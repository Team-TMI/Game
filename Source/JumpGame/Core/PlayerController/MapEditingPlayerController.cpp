// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditingPlayerController.h"

#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "Kismet/GameplayStatics.h"

AMapEditingPlayerController::AMapEditingPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bShowMouseCursor = true;
}

FVector AMapEditingPlayerController::GetMouseWorldPosition(FHitResult& HitResult) const
{
	FVector MouseWorldPosition = FVector::ZeroVector;

	// FVector Start = GetPosition
	FVector2D MouseScreenPosition;
	bool bIsValid = GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y);

	// 마우스가 화면 밖에 있으면 무시
	if (!bIsValid) return FVector::ZeroVector;

	AMapEditingPawn* MapEditingPawn = Cast<AMapEditingPawn>(GetPawn());
	APrimitiveProp* ControlledActor = MapEditingPawn->GetControlledActor();
	
	GetHitResultAtScreenPosition(MouseScreenPosition, ECC_Visibility, true, HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		MouseWorldPosition = HitResult.Location;
		// HitResult가 Gizmo에 걸린 경우 그대로 반환
		if (HitResult.GetActor() == ControlledActor)
		{
			DrawDebugSphere(GetWorld(), MouseWorldPosition, 10.f, 12, FColor::Red, false, 5.f);
			return MouseWorldPosition;
		}
	}

	// 이미 조작하고 있는 오브젝트가 있다면 해당 오브젝트의 Collision을 다시 켜줌
	// 그리고 다시 체크
	if (ControlledActor) ControlledActor->SetCollision(true);
	
	GetHitResultAtScreenPosition(MouseScreenPosition, ECC_Visibility, true, HitResult);
	
	if (ControlledActor) ControlledActor->SetCollision(false);
	
	if (HitResult.IsValidBlockingHit())
	{
		MouseWorldPosition = HitResult.Location;
		DrawDebugSphere(GetWorld(), MouseWorldPosition, 10.f, 12, FColor::Red, false, 5.f);
		return MouseWorldPosition;
	}

	// 그 외에 공중에 마우스가 있는 경우
	FVector MouseLocation;
	FVector MouseDirection;
	UGameplayStatics::DeprojectScreenToWorld(this, MouseScreenPosition, MouseLocation, MouseDirection);
	MouseWorldPosition = MouseLocation + (MouseDirection * 1000.f);

	DrawDebugSphere(GetWorld(), MouseWorldPosition, 10.f, 12, FColor::Red, false, 5.f);
	
	return MouseWorldPosition;
}
