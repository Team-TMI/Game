// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeStartProp.h"


// Sets default values
ABridgeStartProp::ABridgeStartProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABridgeStartProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABridgeStartProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

