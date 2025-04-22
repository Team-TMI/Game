// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ABaseProp::ABaseProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Prop");

	PivotScene = CreateDefaultSubobject<USceneComponent>(TEXT("PivotScene"));
	PivotScene->SetupAttachment(RootComponent);
	
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(50.f));
	CollisionComp->SetupAttachment(PivotScene);
	CollisionComp->SetCollisionProfileName(TEXT("Prop"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionProfileName(TEXT("Prop"));
}

// Called when the game starts or when spawned
void ABaseProp::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseProp::SetCollision(bool bEnable)
{
	if (bEnable)
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called every frame
void ABaseProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

