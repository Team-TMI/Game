// Fill out your copyright notice in the Description page of Project Settings.


#include "DefeatPlatform.h"


// Sets default values
ADefeatPlatform::ADefeatPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("DefeatPlatform");
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
			TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	MeshComp->SetRelativeScale3D(FVector(10,10,0.5));
}

// Called when the game starts or when spawned
void ADefeatPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefeatPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

