// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerTwoProp.h"


// Sets default values
AFlowerTwoProp::AFlowerTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Flower-02.SM-Flower-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AFlowerTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowerTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

