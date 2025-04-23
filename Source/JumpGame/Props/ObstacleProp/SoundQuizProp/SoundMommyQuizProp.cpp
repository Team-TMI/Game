// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMommyQuizProp.h"

#include "Blueprint/UserWidget.h"
#include "JumpGame/UI/Obstacle/SoundQuizClear.h"
#include "JumpGame/UI/Obstacle/SoundQuizFail.h"
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

	// 20번 넘으면 자동 게임 종료, 디버프를 받는다 (못맞춤)
	if (SendResponseIdx >= 20)
	{
		// UI 지우자
		SoundQuizUI->RemoveFromParent();
		bIsQuizFail = true;
		// 실패...
		SoundQuizFail = CreateWidget<USoundQuizFail>(GetWorld(), SoundQuizFailUIClass);
		if (SoundQuizFail)
		{
			SoundQuizFail->AddToViewport();
		}
		SendEndSoundQuizNotify();

		GetWorld()->GetTimerManager().SetTimer(UIRemoveTimerHandle, this, &ASoundMommyQuizProp::RemoveSoundQuizUI, 3.0f, false);
	}

	// 20번 안에, Fin되는 경우 -> 유사도가 높을때
	if (SendResponseIdx < 20 && Similarity >= 90)
	{
		// UI 지우자
		SoundQuizUI->RemoveFromParent();
		// 성공!
		SoundQuizClear = CreateWidget<USoundQuizClear>(GetWorld(), SoundQuizClearUIClass);
		if (SoundQuizClear)
		{
			SoundQuizClear->AddToViewport();
		}
		SendEndSoundQuizNotify();
		
		GetWorld()->GetTimerManager().SetTimer(UIRemoveTimerHandle, this, &ASoundMommyQuizProp::RemoveSoundQuizUI, 3.0f, false);
	}

	FFastLogger::LogConsole(TEXT("SendResponseIdx: %d"), SendResponseIdx);
}

void ASoundMommyQuizProp::SendEndSoundQuizNotify()
{
	Super::SendEndSoundQuizNotify();
	
	SoundQuizUI->RemoveFromParent();
}

void ASoundMommyQuizProp::RemoveSoundQuizUI()
{
	if (SoundQuizClear)
	{
		SoundQuizClear->RemoveFromParent();
	}

	if (SoundQuizFail)
	{
		SoundQuizFail->RemoveFromParent();
	}
}

