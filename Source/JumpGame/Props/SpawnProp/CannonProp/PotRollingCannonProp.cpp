// Fill out your copyright notice in the Description page of Project Settings.


#include "PotRollingCannonProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APotRollingCannonProp::APotRollingCannonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Pot-02.SM-Pot-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("2004"));
}

// Called when the game starts or when spawned
void APotRollingCannonProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotRollingCannonProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

