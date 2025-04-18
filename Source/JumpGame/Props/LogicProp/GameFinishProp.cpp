// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFinishProp.h"

#include <filesystem>

#include "VictoryPlatform.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AGameFinishProp::AGameFinishProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("GameFinish");

	CollisionComp->SetBoxExtent(FVector(60));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
}

void AGameFinishProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	AFrog* Character = Cast<AFrog>(OtherActor);
	if (!bWinnerFound)
	{
		bWinnerFound = true;
		// TODO: 캐릭터의 상태 bIsWin을 만들어서 그 캐릭터만 true로 바꾸기
		// 1등 캐릭터 저장
		WinnerCharacter = Character;

		// 10초 후에 게임을 끝내자
		FTimerHandle EndTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &AGameFinishProp::GameEnd, 10.0f, false);
	}
}

// Called when the game starts or when spawned
void AGameFinishProp::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGameFinishProp::OnMyBeginOverlap);
}

// Called every frame
void AGameFinishProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameFinishProp::GameEnd()
{
	// 물 멈추자
	
	
	// 시상대 스폰 (박스 위치부터 위로)
	FVector VictoryPos = MeshComp->GetComponentLocation() + FVector(0,0,10000);
	AVictoryPlatform* VictoryP = GetWorld()->SpawnActor<AVictoryPlatform>(VictoryPos, FRotator::ZeroRotator);

	// 플레이어 텔레포트
	WinnerCharacter->SetActorLocation(VictoryP->SpawnVictoryCharacter());

	// 카메라 전환
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetViewTargetWithBlend(VictoryP, 1.5f);
	}

	// 게임 종료 처리 (게임모드?)
}

