// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VerticalPropSlotUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UVerticalPropSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	
};
