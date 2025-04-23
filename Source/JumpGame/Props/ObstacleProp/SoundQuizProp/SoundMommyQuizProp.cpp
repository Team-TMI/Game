// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMommyQuizProp.h"

#include "Blueprint/UserWidget.h"
#include "JumpGame/UI/Obstacle/SoundQuizUI.h"


// Sets default values
ASoundMommyQuizProp::ASoundMommyQuizProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoundMommyQuizProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundMommyQuizProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundMommyQuizProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                        SweepResult);

	// 시작하면 UI 띄우자
	SoundQuizUI = CreateWidget<USoundQuizUI>(GetWorld(), SoundQuizUIClass);
	if (SoundQuizUI)
	{
		SoundQuizUI->AddToViewport();
	}
}

void ASoundMommyQuizProp::ReceiveSoundQuizMessage()
{
	Super::ReceiveSoundQuizMessage();
	
	SoundQuizUI->UpdateFromResponse(Similarity, MessageStr);
}

