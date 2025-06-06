// Fill out your copyright notice in the Description page of Project Settings.


#include "HouseTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AHouseTwoProp::AHouseTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-House-02.SM-House-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1021"));
}

// Called when the game starts or when spawned
void AHouseTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHouseTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

