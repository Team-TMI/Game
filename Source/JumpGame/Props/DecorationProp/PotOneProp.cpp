// Fill out your copyright notice in the Description page of Project Settings.


#include "PotOneProp.h"


// Sets default values
APotOneProp::APotOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APotOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

