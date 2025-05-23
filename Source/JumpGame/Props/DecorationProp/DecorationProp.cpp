// Fill out your copyright notice in the Description page of Project Settings.


#include "DecorationProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ADecorationProp::ADecorationProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComp->SetCollisionProfileName(TEXT("OverlapProp"));
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// 그리드 충돌 허용
	bEnableCollision = false;
}

// Called when the game starts or when spawned
void ADecorationProp::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = false;
	SetReplicateMovement(false);
}

// Called every frame
void ADecorationProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

