// Fill out your copyright notice in the Description page of Project Settings.


#include "CursorManager.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void UCursorManager::SetCursor(UObject* WorldContextObject, ECursorName CursorType)
{
	EMouseCursor::Type Type = EMouseCursor::Default;
	FName Name = FName();
	FVector2D HotSpot = FVector2D::ZeroVector;
	
	switch (CursorType)
	{
	case ECursorName::GreenCursor:
		Name = FName("GreenCursor");
		break;
	case ECursorName::FlyCursor:
		Name = FName("FlyCursor");
		break;
	default:
		Name = FName("GreenCursor");
		break;
	}

	UWidgetBlueprintLibrary::SetHardwareCursor(WorldContextObject, Type, Name, HotSpot);
}
