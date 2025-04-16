// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ABaseProp::ABaseProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetBoxExtent(FVector(BoxExtent));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->ComponentTags.Add("TagName");
}

// Called when the game starts or when spawned
void ABaseProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

