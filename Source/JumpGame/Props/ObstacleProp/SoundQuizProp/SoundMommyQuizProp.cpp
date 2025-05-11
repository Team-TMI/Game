// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMommyQuizProp.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/UI/Obstacle/SoundQuizClear.h"
#include "JumpGame/UI/Obstacle/SoundQuizFail.h"
#include "JumpGame/UI/Obstacle/SoundQuizUI.h"
#include "JumpGame/UI/Obstacle/TimeRemainUI.h"


// Sets default values
ASoundMommyQuizProp::ASoundMommyQuizProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/SoundMommy/SM_Radio.SM_Radio"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("2003"));
}

// Called when the game starts or when spawned
void ASoundMommyQuizProp::BeginPlay()
{
	Super::BeginPlay();
	SoundQuizUI = CreateWidget<USoundQuizUI>(GetWorld(), SoundQuizUIClass);
	SoundQuizUI->SetVoiceRecorderComponent(VoiceRecorderComponent);
	SoundQuizFail = CreateWidget<USoundQuizFail>(GetWorld(), SoundQuizFailUIClass);
	SoundQuizClear = CreateWidget<USoundQuizClear>(GetWorld(), SoundQuizClearUIClass);
	TimeRemainUI = CreateWidget<UTimeRemainUI>(GetWorld(), TimeRemainUIClass);
	
	// 미션 종료 시 실행할 함수 바인딩
	TimeRemainUI->OnMissionTimerEnd.AddDynamic(this, &ASoundMommyQuizProp::StopRecord);
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
	if (SoundQuizUI)
	{
		SoundQuizUI->AddToViewport();
	}
}

void ASoundMommyQuizProp::ReceiveSoundQuizMessage()
{
	// 20번 넘으면 자동 게임 종료, 디버프를 받는다 (못맞춤)
	if (SendResponseIdx >= 20)
	{
		// UI 지우자
		SoundQuizUI->RemoveFromParent();
		// 실패...
		if (SoundQuizFail)
		{
			SoundQuizFail->AddToViewport();
		}

		// 퀴즈 끝났다고 알리자!
		SendEndSoundQuizNotify();

		GetWorld()->GetTimerManager().SetTimer(UIRemoveTimerHandle, this, &ASoundMommyQuizProp::RemoveSoundQuizUI, 3.0f, false);
		return;
	}

	// TODO: 정답과 일치할때로 변경해야함
	// 20번 안에, Fin되는 경우 -> 유사도가 높을때
	if (SendResponseIdx < 20)
	{
		if (bSuccess == 1 || Similarity*100 >= 89)
		{
			// UI 지우자
			SoundQuizUI->RemoveFromParent();
			// 성공!
			if (SoundQuizClear)
			{
				SoundQuizClear->AddToViewport();
			}

			// 퀴즈 끝났다고 알리자!
			SendEndSoundQuizNotify();
			
			GetWorld()->GetTimerManager().SetTimer(UIRemoveTimerHandle, this, &ASoundMommyQuizProp::RemoveSoundQuizUI, 3.0f, false);
			return;
		}
	}
	
	Super::ReceiveSoundQuizMessage();
	SoundQuizUI->UpdateFromResponse(Similarity*100, MessageStr);
}

void ASoundMommyQuizProp::SendSoundQuizMessage()
{
	Super::SendSoundQuizMessage();
	
}

void ASoundMommyQuizProp::SendEndSoundQuizNotify()
{
	Super::SendEndSoundQuizNotify();
	
	SoundQuizUI->RemoveFromParent();
	TimeRemainUI->RemoveFromParent();
}

void ASoundMommyQuizProp::StartRecord()
{
	Super::StartRecord();

	// 남은 시간 UI
	TimeRemainUI->AddToViewport();
	TimeRemainUI->StartMissionTimer(3.0f);

	// 버튼 이름 바꾸고
	SoundQuizUI->Text_VoiceSend->SetText(FText::FromString(TEXT("대답 녹음중!")));
	// 버튼 비활성화
	SoundQuizUI->Btn_VoiceSend->SetIsEnabled(false);
}

void ASoundMommyQuizProp::StopRecord()
{
	Super::StopRecord();

	TimeRemainUI->RemoveFromParent();
	SoundQuizUI->Text_VoiceSend->SetText(FText::FromString(TEXT("엄마 말 듣는중...")));
	// 버튼 다시 활성화
	SoundQuizUI->Btn_VoiceSend->SetIsEnabled(true);
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


