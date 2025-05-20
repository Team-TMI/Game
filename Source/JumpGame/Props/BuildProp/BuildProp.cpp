// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ABuildProp::ABuildProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

// Called when the game starts or when spawned
void ABuildProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

