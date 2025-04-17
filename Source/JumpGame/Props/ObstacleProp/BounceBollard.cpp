// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceBollard.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"


// Sets default values
ABounceBollard::ABounceBollard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp->SetBoxExtent(FVector(50));
	MeshComp->SetRelativeLocation(FVector(0, 0, 0));
}

// Called when the game starts or when spawned
void ABounceBollard::BeginPlay()
{
	Super::BeginPlay();
	
	// 콜리전 설정 (Block)
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void ABounceBollard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABounceBollard::CalculateForce(AFrog* Character)
{
	FVector CharacterDir = Character->GetVelocity().GetSafeNormal();
	FVector Direction = CharacterDir*(-1);
	DrawDebugLine(GetWorld(),GetActorLocation(),GetActorLocation()+Direction*200,FColor::Red, false, 3.0f, 0, 2.f);
	float Force = 1000;
	
	Super::LaunchCharacter(Character, Direction, Force);
}

