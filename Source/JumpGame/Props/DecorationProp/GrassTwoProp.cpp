// Fill out your copyright notice in the Description page of Project Settings.


#include "GrassTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AGrassTwoProp::AGrassTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/grass_02.grass_02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	PropDataComponent->SetPropID(TEXT("4018"));
}

// Called when the game starts or when spawned
void AGrassTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrassTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

