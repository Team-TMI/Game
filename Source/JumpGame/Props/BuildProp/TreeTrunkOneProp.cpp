// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeTrunkOneProp.h"


// Sets default values
ATreeTrunkOneProp::ATreeTrunkOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/tree_trunk_01.tree_trunk_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 1, 1));
}

// Called when the game starts or when spawned
void ATreeTrunkOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeTrunkOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

