// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundQuizProp.h"
#include "SoundMommyQuizProp.generated.h"

UCLASS()
class JUMPGAME_API ASoundMommyQuizProp : public ASoundQuizProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundMommyQuizProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void ReceiveSoundQuizMessage() override;
public:
	// UI 관련
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USoundQuizUI> SoundQuizUIClass;
	UPROPERTY(editAnywhere)
	USoundQuizUI* SoundQuizUI;
	
};
