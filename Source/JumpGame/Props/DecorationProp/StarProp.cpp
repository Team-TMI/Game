// Fill out your copyright notice in the Description page of Project Settings.


#include "StarProp.h"


// Sets default values
AStarProp::AStarProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/star.star"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AStarProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStarProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

