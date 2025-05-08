// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateHammerProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ARotateHammerProp::ARotateHammerProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("RotateHammer");

	CollisionComp->SetRelativeLocation(FVector(0, 150, 0));
	// CollisionComp랑 충돌
	// CollisionComp->SetCollisionProfileName(TEXT("Prop"));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));

	PropDataComponent->SetPropID(TEXT("5005"));
}

// Called when the game starts or when spawned
void ARotateHammerProp::BeginPlay()
{
	Super::BeginPlay();
	
	RotAxis = FRotator(0,0,-1);
	RotAngle = 200.f;
}

// Called every frame
void ARotateHammerProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotateHammerProp::CalculateForce(AFrog* Character)
{
	FVector Direction = CollisionComp->GetUpVector();
	float Force = 1000;
	
	Super::LaunchCharacter(Character, Direction, Force, true, true);
}
