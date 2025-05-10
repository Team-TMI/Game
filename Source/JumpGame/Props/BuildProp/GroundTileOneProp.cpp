// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTileOneProp.h"


// Sets default values
AGroundTileOneProp::AGroundTileOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/ground_02_tile.ground_02_tile"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AGroundTileOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundTileOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

