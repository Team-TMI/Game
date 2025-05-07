// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefeatPlatform.h"
#include "LogicProp.h"
#include "VictoryPlatform.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/UI/VictoryPageUI.h"
#include "GameFinishProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API AGameFinishProp : public ALogicProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameFinishProp();

protected:
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                      const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	class UJumpGameInstance* GI;
	
	// 첫번째 플레이어가 들어오면 10초후 게임 끝내자
	UFUNCTION()
	void GameEnd();
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_GameEnd();
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_ShowClearUI();

	// 첫번째 플레이어 저장 (1등)
	UPROPERTY(Replicated)
	class AFrog* WinnerCharacter;
	UPROPERTY()
	AFrog* Character;
	
	// 1등 정해졌나요?
	UPROPERTY()
	bool bWinnerFound = false;

	// 시상대
	UPROPERTY(Replicated)
	AVictoryPlatform* VictoryP;
	UPROPERTY()
	ADefeatPlatform* DefeatP;

	// UI관련
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	TSubclassOf<class UVictoryPageUI> VictoryPageUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UVictoryPageUI* VictoryPageUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USoundQuizClear> SoundQuizClearUIClass;
	UPROPERTY(EditAnywhere)
	USoundQuizClear* SoundQuizClear;

};
