// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTileTwoProp.h"


// Sets default values
AGroundTileTwoProp::AGroundTileTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/ground_02.ground_02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AGroundTileTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundTileTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

