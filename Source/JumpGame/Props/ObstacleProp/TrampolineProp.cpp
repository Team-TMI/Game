// Fill out your copyright notice in the Description page of Project Settings.


#include "TrampolineProp.h"

#include <filesystem>

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"


// Sets default values
ATrampolineProp::ATrampolineProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TagName = "Trampoline";
}

// Called when the game starts or when spawned
void ATrampolineProp::BeginPlay()
{
	Super::BeginPlay();

	// 콜리전 설정 (Block)
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void ATrampolineProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrampolineProp::LaunchCharacter(ACharacter* Character)
{
	// 위로 띄우기 테스트
	// TODO: 변수 빼기
	FVector CharacterDir = Character->GetVelocity().GetSafeNormal();
	LaunchVelocity = CharacterDir*300 + FVector(0, 0, 500);
	Super::LaunchCharacter(Character);
}

