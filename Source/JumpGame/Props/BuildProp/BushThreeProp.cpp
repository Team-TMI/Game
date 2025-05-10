// Fill out your copyright notice in the Description page of Project Settings.


#include "BushThreeProp.h"


// Sets default values
ABushThreeProp::ABushThreeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/bush_03.bush_03"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ABushThreeProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABushThreeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

