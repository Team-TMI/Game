// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGroundProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ABasicGroundProp::ABasicGroundProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Ground");
	Tags.Add("BasicGround");
	
	CollisionComp->SetCollisionProfileName(TEXT("Prop"));
	MeshComp->SetCollisionProfileName(TEXT("Prop"));

	PropDataComponent->SetPropID(TEXT("1000"));
}

// Called when the game starts or when spawned
void ABasicGroundProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicGroundProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

