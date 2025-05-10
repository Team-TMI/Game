// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTwoProp.h"


// Sets default values
AGroundTwoProp::AGroundTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGroundTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

