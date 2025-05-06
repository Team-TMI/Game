// Fill out your copyright notice in the Description page of Project Settings.


#include "BrightFrog.h"

#include "Components/BoxComponent.h"


// Sets default values
ABrightFrog::ABrightFrog()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Decoration");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Game/Props/Decoration/BrightFrog/SM_BrightFrog.SM_BrightFrog"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	
}

// Called when the game starts or when spawned
void ABrightFrog::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABrightFrog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
