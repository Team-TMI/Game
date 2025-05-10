// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeSevenProp.h"


// Sets default values
ATreeSevenProp::ATreeSevenProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Tree-05.SM-Tree-05"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 5));
}

// Called when the game starts or when spawned
void ATreeSevenProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeSevenProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

