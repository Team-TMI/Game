// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeTrunkTwoProp.h"


// Sets default values
ATreeTrunkTwoProp::ATreeTrunkTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATreeTrunkTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeTrunkTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

