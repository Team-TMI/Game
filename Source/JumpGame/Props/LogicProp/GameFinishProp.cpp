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
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/UI/Obstacle/SoundQuizClear.h"
#include "JumpGame/UI/Obstacle/SoundQuizFail.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AGameFinishProp::AGameFinishProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("GameFinish");

	CollisionComp->SetBoxExtent(FVector(50,60,80));
	CollisionComp->SetRelativeScale3D(FVector(3, 2, 1));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));

	Super::SetSize(FVector(3,2,1));

	PropDataComponent->SetPropID(TEXT("9000"));
}

void AGameFinishProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Frog")) return;
	if (bIsActive == false) return;
		
	Character = Cast<AFrog>(OtherActor);
	if (!bWinnerFound)
	{
		bWinnerFound = true;
		
		// 1등 캐릭터 저장
		WinnerCharacter = Character;

		// 1등 캐릭터에게 Clear UI 띄우자
		MulticastRPC_ShowClearUI();
		
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
	Character = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());

	SoundQuizClear = CreateWidget<USoundQuizClear>(GetWorld(), SoundQuizClearUIClass);
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGameFinishProp::OnMyBeginOverlap);
}

void AGameFinishProp::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameFinishProp, WinnerCharacter);
	DOREPLIFETIME(AGameFinishProp, VictoryP);
}

// Called every frame
void AGameFinishProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameFinishProp::GameEnd()
{
	// 시상대 스폰 (박스 위치부터 위로)
	FVector VictoryPos = MeshComp->GetComponentLocation() + FVector(0,0,20000);
	FVector DefeatPos = MeshComp->GetComponentLocation() + FVector(0,0,10000);
	VictoryP = GetWorld()->SpawnActor<AVictoryPlatform>(VictoryPos, FRotator::ZeroRotator);
	DefeatP = GetWorld()->SpawnActor<ADefeatPlatform>(DefeatPos, FRotator::ZeroRotator);

	// 우승자 앞에 보게 정렬하기
	WinnerCharacter->SetActorRotation(FRotator(0, 90, 0));
	// 플레이어 텔레포트
	WinnerCharacter->SetActorLocation(VictoryP->SpawnVictoryCharacter());
	if (IsValid(Character) && Character != WinnerCharacter)
	{
		Character->SetActorLocation(DefeatP->SpawnDefeatCharacter());
	}
	
	// 게임 끝
	MulticastRPC_GameEnd();

	// 물 멈추자
	ARisingWaterProp* RisingWaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));
	if (RisingWaterProp)
	{
		RisingWaterProp->StopRising();
	}
	
	// TODO: 게임 종료 처리 - 게임 한판 진행 시간 멈추기 (게임모드?)
}

void AGameFinishProp::MulticastRPC_ShowClearUI_Implementation()
{
	// 내 캐릭터가 위너인지 판단하자
	AFrog* LocalCharacter = Cast<AFrog>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (LocalCharacter == WinnerCharacter)
	{
		if (SoundQuizClear)
		{
			SoundQuizClear->AddToViewport();
		}
		LocalCharacter->SetJumpGaugeVisibility(false);
	}
}

void AGameFinishProp::MulticastRPC_GameEnd_Implementation()
{
	// UI 있으면 지우기
	if (SoundQuizClear)
	{
		SoundQuizClear->RemoveFromParent();
	}
	
	// 플레이어들 조작막기
	AFrog* LocalCharacter = Cast<AFrog>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	LocalCharacter->StopMovementAndResetRotation();
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
	
	// 우승자 앞에 보게 정렬하기
	WinnerCharacter->SetActorRotation(FRotator(0, 90, 0));

	// 카메라 정렬
	PC->SetViewTargetWithBlend(VictoryP, 0.5f);
	
	// UI를 띄우자
	VictoryPageUI = CreateWidget<UVictoryPageUI>(GetWorld(), VictoryPageUIClass);
	if (VictoryPageUI)
	{
		VictoryPageUI->SetVictoryPlayerName(WinnerCharacter->GetName());
		VictoryPageUI->AddToViewport();
	}
}

