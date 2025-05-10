// Fill out your copyright notice in the Description page of Project Settings.


#include "BushOneProp.h"


// Sets default values
ABushOneProp::ABushOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/bush_01.bush_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ABushOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABushOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

