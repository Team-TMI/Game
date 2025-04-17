// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateHammerProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"


// Sets default values
ARotateHammerProp::ARotateHammerProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("RotateHammer");
}

// Called when the game starts or when spawned
void ARotateHammerProp::BeginPlay()
{
	Super::BeginPlay();

	// 콜리전 설정 (Block)
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RotAxis = FRotator(0,0,1);
	RotAngle = 50.f;
}

// Called every frame
void ARotateHammerProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotateHammerProp::CalculateForce(AFrog* Character)
{
	FVector CharacterDir = Character->GetVelocity().GetSafeNormal();
	FVector Direction = CharacterDir + FVector::UpVector;
	float Force = 300;
	
	Super::LaunchCharacter(Character, Direction, Force);
}
