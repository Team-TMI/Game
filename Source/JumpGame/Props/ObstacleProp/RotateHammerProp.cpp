// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateHammerProp.h"


// Sets default values
ARotateHammerProp::ARotateHammerProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARotateHammerProp::BeginPlay()
{
	Super::BeginPlay();

	RotateSpeed = 50.f;
}

// Called every frame
void ARotateHammerProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotateHammerProp::LaunchCharacter(ACharacter* Character)
{
	Super::LaunchCharacter(Character);
}

