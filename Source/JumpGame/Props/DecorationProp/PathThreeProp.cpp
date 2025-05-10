// Fill out your copyright notice in the Description page of Project Settings.


#include "PathThreeProp.h"


// Sets default values
APathThreeProp::APathThreeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APathThreeProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathThreeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

