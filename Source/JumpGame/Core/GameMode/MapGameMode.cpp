// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Props/LogicProp/GameStartProp.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"
#include "Kismet/GameplayStatics.h"

AMapGameMode::AMapGameMode()
{
	LoadMapComponent = CreateDefaultSubobject<ULoadMapComponent>(TEXT("LoadMapComponent"));
	SaveMapComponent = CreateDefaultSubobject<USaveMapComponent>(TEXT("SaveMapComponent"));
}

void AMapGameMode::BeginPlay()
{
	Super::BeginPlay();
}

AActor* AMapGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// 태그로 찾기
	TArray<AActor*> FoundProps;
	UGameplayStatics::GetAllActorsWithTag(this, FName("GameStart"), FoundProps);
	AGameStartProp* Prop = Cast<AGameStartProp>(FoundProps[0]);
	
	APlayerStart* TempStart = GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), Prop->PlayerStartTransform());
	return TempStart;
}

