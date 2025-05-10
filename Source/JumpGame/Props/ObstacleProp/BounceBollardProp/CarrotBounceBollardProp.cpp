// Fill out your copyright notice in the Description page of Project Settings.


#include "CarrotBounceBollardProp.h"


// Sets default values
ACarrotBounceBollardProp::ACarrotBounceBollardProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM_BounceBollard.SM_BounceBollard"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(1, 1, 2));
}

// Called when the game starts or when spawned
void ACarrotBounceBollardProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarrotBounceBollardProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

