// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFinishProp.h"

#include <filesystem>

#include "DefeatPlatform.h"
#include "RisingWaterProp.h"
#include "VictoryPlatform.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGameFinishProp::AGameFinishProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("GameFinish");

	CollisionComp->SetBoxExtent(FVector(60.f));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

void AGameFinishProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (!OtherActor->ActorHasTag("Frog")) return;
	FFastLogger::LogConsole(TEXT("Overlap!!!: %s"), *OtherActor->GetName());
	
	AFrog* Character = Cast<AFrog>(OtherActor);
	if (!bWinnerFound)
	{
		bWinnerFound = true;
		
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
	
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());

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
	ARisingWaterProp* RisingWaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));
	if (RisingWaterProp)
	{
		RisingWaterProp->StopRising();
	}
	
	// 플레이어들 조작막기
	// TODO: 캐릭터 멈추는 함수 넣기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
	// 우승자 앞에 보게 정렬하기
	WinnerCharacter->SetActorRotation(FRotator(0, 90, 0));
	
	// 시상대 스폰 (박스 위치부터 위로)
	FVector VictoryPos = MeshComp->GetComponentLocation() + FVector(0,0,10000);
	FVector DefeatPos = MeshComp->GetComponentLocation() + FVector(0,0,20000);
	AVictoryPlatform* VictoryP = GetWorld()->SpawnActor<AVictoryPlatform>(VictoryPos, FRotator::ZeroRotator);
	ADefeatPlatform* DefeatP = GetWorld()->SpawnActor<ADefeatPlatform>(DefeatPos, FRotator::ZeroRotator);
	
	// 플레이어 텔레포트
	WinnerCharacter->SetActorLocation(VictoryP->SpawnVictoryCharacter());
	AFrog* Character = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Character != WinnerCharacter)
	{
		SetActorLocation(DefeatP->SpawnDefeatCharacter());
	}
	
	// 카메라 전환
	if (PC)
	{
		PC->SetViewTargetWithBlend(VictoryP, 0.2f);
	}

	// UI를 띄우자
	VictoryPageUI = CreateWidget<UVictoryPage>(GetWorld(), VictoryPageUIClass);
	if (VictoryPageUI)
	{
		VictoryPageUI->SetVictoryPlayerName(WinnerCharacter->GetName());
		VictoryPageUI->AddToViewport();
	}

	// TODO: 게임 종료 처리 - 게임 한판 진행 시간 멈추기 (게임모드?)
}

