// Fill out your copyright notice in the Description page of Project Settings.


#include "HouseThreeProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AHouseThreeProp::AHouseThreeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-House-03.SM-House-03"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1022"));
}

// Called when the game starts or when spawned
void AHouseThreeProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHouseThreeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

