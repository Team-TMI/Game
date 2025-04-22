// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateSpikeProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ARotateSpikeProp::ARotateSpikeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp->SetupAttachment(RootComponent);
	
	Hammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hammer"));
	Hammer->SetupAttachment(MeshComp);
	Hammer->SetRelativeLocation(FVector(0, 160, 0));
	CollisionComp->SetBoxExtent(FVector(60.f));
	CollisionComp->SetRelativeLocation(FVector::ZeroVector);
	CollisionComp->SetupAttachment(Hammer);

	Spike = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spike"));
	Spike->SetupAttachment(MeshComp);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempCylinder(TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleCylinder.SM_ObstacleCylinder'"));
	if (TempCylinder.Succeeded())
	{
		MeshComp->SetStaticMesh(TempCylinder.Object);
	}
}

// Called when the game starts or when spawned
void ARotateSpikeProp::BeginPlay()
{
	Super::BeginPlay();

	SwingHammer();
}

// Called every frame
void ARotateSpikeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotateSpikeProp::CalculateForce(AFrog* Character)
{
	Super::CalculateForce(Character);
}

void ARotateSpikeProp::SwingHammer_Implementation()
{
}

void ARotateSpikeProp::ReverseSwingHammer_Implementation()
{
}
