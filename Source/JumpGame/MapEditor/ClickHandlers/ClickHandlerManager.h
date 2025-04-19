// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClickHandlerInterface.h"
#include "Components/ActorComponent.h"
#include "ClickHandlerManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UClickHandlerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UClickHandlerManager();

	void RegisterHandler(TSharedPtr<IClickHandler> Handler);
	bool HandleClick(class AMapEditingPlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;

private:
	// GC의 관리 대상이 아님
	// 주의 : 내부적으로 UObject를 캐싱하지 않기 : 즉 일회성으로만 사용
	TArray<TSharedPtr<IClickHandler>> Handlers;

	UPROPERTY()
	FClickResponse ControlledClickResponse;
};
