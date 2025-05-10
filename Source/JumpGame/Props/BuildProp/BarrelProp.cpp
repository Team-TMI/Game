// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelProp.h"


// Sets default values
ABarrelProp::ABarrelProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/barrel.barrel"));
	if (MeshAsset.Succeeded())
    {
		MeshComp->SetStaticMesh(MeshAsset.Object);
    }
}

// Called when the game starts or when spawned
void ABarrelProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrelProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

