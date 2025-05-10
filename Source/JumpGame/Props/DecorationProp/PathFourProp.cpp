// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFourProp.h"


// Sets default values
APathFourProp::APathFourProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APathFourProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathFourProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

