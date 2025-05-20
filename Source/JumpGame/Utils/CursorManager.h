// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CursorManager.generated.h"

UENUM(BlueprintType)
enum class ECursorName : uint8
{
	GreenCursor UMETA(DisplayName = "GreenCursor"),
	FlyCursor UMETA(DisplayName = "FlyCursor")
};

UCLASS()
class JUMPGAME_API UCursorManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void SetCursor(UObject* WorldContextObject, ECursorName CursorType);
};
