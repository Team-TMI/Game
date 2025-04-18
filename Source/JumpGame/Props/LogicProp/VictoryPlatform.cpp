// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPlatform.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AVictoryPlatform::AVictoryPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 밑바닥
	MeshComp->SetRelativeLocation(FVector(0, 0, -80));
	MeshComp->SetRelativeScale3D(FVector(10,5,0.5));

	// 1등 발판
	VictoryCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryCube"));
	VictoryCube->SetRelativeScale3D(FVector(1.5,1.1,1.1));
	VictoryCube->SetupAttachment(CollisionComp);

	// 카메라
	VictoryCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VictoryCamera"));
	VictoryCamera->SetupAttachment(CollisionComp);
	VictoryCamera->FieldOfView = 50.0f;
	VictoryCamera->SetRelativeLocation(FVector(0, 580, 110));
	VictoryCamera->SetRelativeRotation(FRotator(0, -90, 0));

	// 배경
	VictoryPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VictoryPlane"));
	VictoryPlane->SetupAttachment(MeshComp);
	VictoryPlane->SetRelativeLocation(FVector(0, -50, 500));
	VictoryPlane->SetRelativeRotation(FRotator(0, 0, 90));
	VictoryPlane->SetRelativeScale3D(FVector(3, 1.5, 1));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempPlane(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstaclePlane.SM_ObstaclePlane'"));
	if (TempPlane.Succeeded())
	{
		VictoryPlane->SetStaticMesh(TempPlane.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempCube(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempPlane.Succeeded())
	{
		VictoryCube->SetStaticMesh(TempMesh.Object);
	}
	
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

FVector AVictoryPlatform::SpawnVictoryCharacter()
{
	FVector SpawnLocation = VictoryCube->GetComponentLocation() + FVector(0.f, 0.f, 200.f);
	return SpawnLocation;
}

