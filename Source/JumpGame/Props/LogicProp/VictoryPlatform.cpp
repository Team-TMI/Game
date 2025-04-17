// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPlatform.h"


// Sets default values
AVictoryPlatform::AVictoryPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/ObstacleBaseCube.ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	MeshComp->SetRelativeScale3D(FVector(3,3,1));
	
	Tags.Add("VictoryPlatform");
}

// Called when the game starts or when spawned
void AVictoryPlatform::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AVictoryPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

