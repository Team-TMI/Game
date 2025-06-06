// Fill out your copyright notice in the Description page of Project Settings.


#include "PathNineProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathNineProp::APathNineProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-09.SM-Path-09"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	PropDataComponent->SetPropID(TEXT("4029"));
}

// Called when the game starts or when spawned
void APathNineProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathNineProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

