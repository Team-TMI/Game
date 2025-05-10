// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorProp.h"


// Sets default values
ADoorProp::ADoorProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Door-01.SM-Door-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(1, 2, 2));
}

// Called when the game starts or when spawned
void ADoorProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

