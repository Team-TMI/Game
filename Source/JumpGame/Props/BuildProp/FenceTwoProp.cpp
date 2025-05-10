// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceTwoProp.h"


// Sets default values
AFenceTwoProp::AFenceTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Fence-02.SM-Fence-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(1, 2, 1));
}

// Called when the game starts or when spawned
void AFenceTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFenceTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

